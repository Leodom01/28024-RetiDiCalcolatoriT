package EsStream;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {

	public static void main(String args[]) {
		
		try {
			Socket socket = new Socket(InetAddress.getByName("localhost"), 9000);
			DataOutputStream sockOut = new DataOutputStream(socket.getOutputStream());
			DataInputStream sockIn = new DataInputStream(socket.getInputStream());
			
			BufferedReader inReader = new BufferedReader(new InputStreamReader(System.in));
			String readLine = null; 
			
			System.out.println("Inserisci il nome del file di cui contare i caratteri");
			while((readLine = inReader.readLine()) != null) {
				BufferedReader buffReader = new BufferedReader(new FileReader(readLine));
				//Scrivo nome file
				sockOut.writeUTF(readLine);
				int rowInFile = 0;
				String toSend;
				while((toSend = buffReader.readLine()) != null) {
					sockOut.writeUTF(toSend);
					rowInFile++;
				}	
				System.out.println("Scritto tutto su socket out");
				System.out.println("Ora esco, ciao ciao");
				
				buffReader.close();
			}
			socket.close();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		
	}
	
}
