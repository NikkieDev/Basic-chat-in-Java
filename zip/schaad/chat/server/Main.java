package zip.schaad.chat.server;

import java.io.IOException;

public class Main
{
	public static void main(String[] args)
	{
		Server server;

		try {
			server = new Server(8080);
		} catch (IOException e) {
			System.out.println("Unable to create server socket, killing process.");
			return;
		}

		server.listen();
	}
}
