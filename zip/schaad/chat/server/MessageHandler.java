package zip.schaad.chat.server;

import java.util.HashMap;

class MessageHandler
{
    private static final MessageHandler instance = new MessageHandler();
    private final HashMap<Integer, ClientHandler> subscribers = new HashMap<>();
    private int subscriberIdCounter = 1;

    private MessageHandler() {};

    public static MessageHandler getInstance()
    {
        return instance;
    }

    public int subscribe(ClientHandler handler)
    {
        int subscriberId = this.subscriberIdCounter;
        this.subscribers.put(subscriberId, handler);
        this.subscriberIdCounter++;

        if (this.subscribers.size() >= 2) {
            System.out.println("reached 2 users");
            this.sendToAllSubscribers("Hello all! We currently have " + String.valueOf(this.subscribers.size()) + " connections", true);
        }

        return subscriberId;
    }

    public void unsubscribe(int id)
    {
        if (this.subscribers.get(id) != null) {
            this.subscribers.remove(id);
        }
    }

    public HashMap<Integer, ClientHandler> getAllSubscribers()
    {
        return this.subscribers;
    }

    public void sendToAllSubscribers(String message, boolean systemMessage)
    {
        for (int i : this.subscribers.keySet()) {
            if (systemMessage) {
                this.subscribers.get(i).sendMessage("[System]: " + String.valueOf(message));
            } else {
                // this.subscribers.get(i).sendMessage(message);
            }

        }
    }
}