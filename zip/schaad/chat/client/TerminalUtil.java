package zip.schaad.chat.client;

class TerminalUtil
{
    public static void clearScreen()
    {
        System.out.println("\033[H\033[2J");
        System.out.flush();
    }

    public static void PrintSystem(String message)
    {
        System.out.println("[System] " + message);
    }
}