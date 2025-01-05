import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class Client {
    public static void main(String[] args) {
        String serverIP = "127.0.0.72";
        int port = 58291;

        if (args.length < 1) {
            System.out.println("Usage: java Client <message>");
            return;
        }

        String message = args[0];

        try (Socket socket = new Socket(serverIP, port);
             OutputStream output = socket.getOutputStream();
             PrintWriter writer = new PrintWriter(output, true)) {

            System.out.println("Connecting to server " + serverIP + ":" + port + "...");
            writer.println(message);
            System.out.println("Message Sent!");
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
