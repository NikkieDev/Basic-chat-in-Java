package zip.schaad.chat.client;

import java.io.IOException;
import java.net.*;

class ServerConnectionHandler
{
    private static final ServerConnectionHandler instance = new ServerConnectionHandler();
    private BidirectionalTrafficService trafficService;
    private Socket serverConnection;

    private ServerConnectionHandler() {};

    public static ServerConnectionHandler getInstance()
    {
        return instance;
    }

    public BidirectionalTrafficService getTrafficService() throws Exception
    {
        if (this.trafficService != null) {
            return this.trafficService;
        }

        throw new Exception("Traffic service not initialized");
    }

    public Socket getConnection() throws Exception
    {
        if (this.serverConnection != null) {
            return this.serverConnection;
        }

        throw new Exception("Not connected to server");
    }

    public void disconnectFromServer()
    {
        TerminalUtil.PrintSystem("Disconnecting...");

        try {
            this.serverConnection.close();
            this.trafficService.close();
        } catch (IOException e) {
            this.serverConnection = null;
        }

        TerminalUtil.PrintSystem("Disconnected from server");
    }

    public void connectToServer(String ipv4, int port) throws IOException, UnknownHostException
    {
        this.serverConnection = new Socket(ipv4, port);
        this.trafficService = new BidirectionalTrafficService(this.serverConnection);

        TerminalUtil.clearScreen();
        TerminalUtil.PrintSystem("Connected to " + ipv4 + ":" + String.valueOf(port));
    }
}