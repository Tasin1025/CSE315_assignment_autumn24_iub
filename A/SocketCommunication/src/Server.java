import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static void main(String[] args) {
        int port = 58291;

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Server hosted at 127.0.0.72:" + port + "!");

            while (true) {
                Socket clientSocket = serverSocket.accept();
                BufferedReader reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                String clientMessage = reader.readLine();

                System.out.println("[" + clientSocket.getInetAddress() + "] " + clientMessage);

                clientSocket.close();
            }
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
