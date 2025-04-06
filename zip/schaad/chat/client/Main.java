package zip.schaad.chat.client;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Main
{
    public static void main(String[] args)
    {        
        ServerConnectionHandler connectionHandler;
        BidirectionalTrafficService trafficService;
        Scanner in = new Scanner(System.in);
        String ipv4;

        int port = 8080;

        System.out.print("Choose a server to connect to (ipv4): ");

        try {
            ipv4 = in.nextLine();

            while (ipv4.equals("")) {
                ipv4 = in.nextLine();
            }

            TerminalUtil.PrintSystem("Connecting to " + ipv4 + ":" + String.valueOf(port));
            
            connectionHandler = ServerConnectionHandler.getInstance();
            connectionHandler.connectToServer(ipv4, port);
            trafficService = connectionHandler.getTrafficService();
        } catch (UnknownHostException e) {
            TerminalUtil.PrintSystem("Unable to find host.");
            return;
        } catch (IOException e) {
            TerminalUtil.PrintSystem("Unable to connect to host.");
            return;
        } catch (Exception e) {
            TerminalUtil.PrintSystem("An unkown error has occured");
            return;
        }

        new Thread(new IncomingTrafficListenerThread(trafficService)).start();
        trafficService.listenForUserInput();
        
        in.close();
        connectionHandler.disconnectFromServer();
    }
}