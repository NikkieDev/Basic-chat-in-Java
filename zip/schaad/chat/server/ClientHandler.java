package zip.schaad.chat.server;

import java.io.*;
import java.net.*;

class ClientHandler implements Runnable
{
    private final static int ACTIVITY_TIMEOUT_IN_MILLISECONDS = 30000;
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
        this.sendMessage("Connected to messages, you are number " + String.valueOf(this.subscriberId));
        MessageHandler.getInstance().sendToAllSubscribers("Client " + this.subscriberId + " has entered the room");
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
        long lastActivityTime = System.currentTimeMillis();

        while (true) {
            if (System.currentTimeMillis() - lastActivityTime >= ACTIVITY_TIMEOUT_IN_MILLISECONDS) {
                System.out.println("Sending keepalive to " + this.subscriberId);

                if (!this.sendKeepAliveSignal()) {
                    break;
                }

                lastActivityTime = System.currentTimeMillis();
                System.out.println("Keepalive heartbeat from " + this.subscriberId);
            }

            try {
                String messageFromClient = this.clientInputStreamBuffered.readLine(); // times out after STREAM_TIMEOUT_IN_MILLISECONDS

                if (messageFromClient != null) {
                    MessageHandler.getInstance().sendToAllSubscribers("[Client "+ this.subscriberId +"]: " + messageFromClient);
                    lastActivityTime = System.currentTimeMillis();
                }
            } catch (IOException e) {}
        }
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
            MessageHandler.getInstance().sendToAllSubscribers("Client " + this.subscriberId + " left the room");
            MessageHandler.getInstance().unsubscribe(this.subscriberId);
            this.tryCloseConnection();
            System.out.println("Client " + this.subscriberId + " has disconnected");
        }
    }
}