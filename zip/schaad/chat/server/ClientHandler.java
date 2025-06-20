package zip.schaad.chat.server;

import java.io.*;
import java.net.*;

class ClientHandler implements Runnable
{
    private final static int ACTIVITY_TIMEOUT_IN_MILLISECONDS = 5000;
    private final static int STREAM_TIMEOUT_IN_MILLISECONDS = 10000;
    
    private Socket clientConnection;
    private PrintWriter clientOutputStream;
    private BufferedReader clientInputStreamBuffered;
    private int subscriberId;

    public ClientHandler(Socket clientConnection) throws SocketException
    {
        System.out.println("Accepted a new connection from " + clientConnection.getRemoteSocketAddress());
        this.clientConnection = clientConnection;
        this.clientConnection.setSoTimeout(STREAM_TIMEOUT_IN_MILLISECONDS);
    }

    private void sendWelcomeMessage()
    {
        this.sendMessage("Welcome to server.");
        this.sendMessage("Connecting you to message server...");
    }

    private void tryCloseConnection()
    {
        try {
            this.clientOutputStream.close();
            this.clientInputStreamBuffered.close();
            this.clientConnection.close();
        } catch (IOException e) {
            System.out.println("Unable to correctly close connection to client, forcing disconnect.");

            this.clientOutputStream = null;
            this.clientInputStreamBuffered = null;
            this.clientConnection = null;
        }
    }

    private void subscribeToMessages()
    {
        this.subscriberId = MessageHandler.getInstance().subscribe(this);
        this.sendMessage("Connected to messages, you are number " + this.subscriberId);
        MessageHandler.getInstance().sendToAllSubscribers("Client " + this.subscriberId + " has entered the room", true);
    }

    private boolean sendKeepAliveSignal() throws IOException
    {
        this.sendMessage("KEEPALIVE");
        String responseFromClient = this.clientInputStreamBuffered.readLine();

        if (responseFromClient == null) {
            System.out.println("No keepalive heartbeat received from " + this.subscriberId);
            return false;
        }

        return true;
    }

    private void acceptTrafficFromClient() throws IOException, InterruptedException
    {
        this.clientInputStreamBuffered = new BufferedReader(new InputStreamReader(this.clientConnection.getInputStream()));
        long lastHeartBeat = System.currentTimeMillis();

        while (true) {
            if (System.currentTimeMillis() - lastHeartBeat >= ACTIVITY_TIMEOUT_IN_MILLISECONDS) {
                if (!this.sendKeepAliveSignal()) {
                    break;
                }

                lastHeartBeat = System.currentTimeMillis();
            }

            try {
                String messageFromClient = this.clientInputStreamBuffered.readLine(); // times out after STREAM_TIMEOUT_IN_MILLISECONDS

                if (messageFromClient.isBlank()) {
                    return;
                }

                switch (messageFromClient) {
                    case "DISCONNECT" -> this.disconnectClient();
                    case "HEARTBEAT" -> lastHeartBeat = System.currentTimeMillis();
                    default -> MessageHandler.getInstance().sendToAllSubscribers("[Client " + this.subscriberId + "]: " + messageFromClient, false);
                }
            } catch (IOException e) {}
        }
    }

    private void disconnectClient()
    {
        System.out.println("Disconnecting client " + this.subscriberId);
        MessageHandler.getInstance().sendToAllSubscribers("Client " + this.subscriberId + " left the room", true);
        MessageHandler.getInstance().unsubscribe(this.subscriberId);
        this.tryCloseConnection();
    }

    public void sendMessage(String message)
    {
        this.clientOutputStream.println(message);
    }

    @Override
    public void run()
    {
        try {
            this.clientOutputStream = new PrintWriter(this.clientConnection.getOutputStream(), true);
            this.sendWelcomeMessage();
            this.subscribeToMessages();
            this.acceptTrafficFromClient();

        } catch (IOException e) {
            System.out.println("[error]: " + e.getMessage());
        } catch (InterruptedException e) {
            System.out.println("Thread interrupted.");
        } finally {
            this.disconnectClient();
            System.out.println("Client " + this.subscriberId + " has disconnected");
        }
    }
}