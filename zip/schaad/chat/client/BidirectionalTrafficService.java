package zip.schaad.chat.client;

import java.io.*;
import java.net.*;
import java.util.NoSuchElementException;
import java.util.Scanner;

class BidirectionalTrafficService
{
    private BufferedReader inboundTrafficBuffered;
    private PrintWriter outboundTraffic;

    public BidirectionalTrafficService(Socket serverConnection) throws IOException
    {
        this.inboundTrafficBuffered = new BufferedReader(new InputStreamReader(serverConnection.getInputStream()));
        this.outboundTraffic = new PrintWriter(serverConnection.getOutputStream(), true);
    }

    public BufferedReader getInboundTraffic()
    {
        return this.inboundTrafficBuffered;
    }

    public PrintWriter getOutboundTraffic()
    {
        return this.outboundTraffic;
    }

    public void listenForUserInput()
    {
        Scanner userMessageReader = new Scanner(System.in);
        
        while (true) {
            String messageToSend;

            try {
                messageToSend = userMessageReader.nextLine();

                if (messageToSend.isBlank()) {
                    continue;
                }

                this.outboundTraffic.println(messageToSend);

                if (messageToSend.equals("DISCONNECT")) {
                    break;
                }
            } catch (NoSuchElementException e) {}
        }
    }

    public void close()
    {
        try {
            this.inboundTrafficBuffered.close();
            this.outboundTraffic.close();
        } catch (IOException e) {
            this.inboundTrafficBuffered = null;
            this.outboundTraffic = null;
        }
        
        TerminalUtil.PrintSystem("Closed traffic");
    }
}