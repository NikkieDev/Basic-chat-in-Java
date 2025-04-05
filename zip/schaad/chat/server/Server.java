package zip.schaad.chat.server;

import java.io.IOException;
import java.net.*;

public class Server
{
    final private int port;
    final private ServerSocket listener;

    public Server(int port) throws IOException
    {
        this.port = port;
        this.listener = new ServerSocket(this.port);
    }

    public void listen()
    {
        while (true) {
            try {
                Socket clientConnection = this.listener.accept();
                ClientHandler clientHandler = new ClientHandler(clientConnection);

                new Thread(clientHandler).start();
            } catch (IOException e) {
                System.out.println("Unable to accept client connection, destroying...");
            }
        }
    }
}