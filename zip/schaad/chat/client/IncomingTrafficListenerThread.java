package zip.schaad.chat.client;

import java.io.IOException;

class IncomingTrafficListenerThread implements Runnable
{
    private final BidirectionalTrafficService trafficService;

    public IncomingTrafficListenerThread(BidirectionalTrafficService trafficService)
    {
        this.trafficService = trafficService;
    }

    @Override
    public void run()
    {
        try {
            String message;

            while ((message = this.trafficService.getInboundTraffic().readLine()) != null) {
                if (message.equals("KEEPALIVE")) {
                    this.trafficService.getOutboundTraffic().println("HEARTBEAT");
                    continue;
                }

                System.out.print("\r");
                System.out.println(message);
            }
        } catch (IOException e) {
            TerminalUtil.PrintSystem("Connection to the server has been lost.");
        }
    }
}