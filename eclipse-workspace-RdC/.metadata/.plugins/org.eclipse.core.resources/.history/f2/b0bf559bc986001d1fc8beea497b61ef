package EsStream;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

	public static void main(String args[]) {
		
		try {
			ServerSocket socket = new ServerSocket(9000);
			socket.setReuseAddress(true);
			
			//Server sequenziale
			while(true) {
				System.out.println("Pronto a leggere una nuova connessione...");
				Socket currentSock = socket.accept();
				DataOutputStream dOutStr = new DataOutputStream(currentSock.getOutputStream());
				DataInputStream dInStr = new DataInputStream(currentSock.getInputStream());
				
				String received = dInStr.readUTF();
				System.out.println("Sto per ricevere il file "+received);
				try {
					while(true) {
						System.out.println(dInStr.readUTF());
					}
				}catch(EOFException e) {
					System.out.println("Fine lettura del file!");
				}
				dOutStr.close();
				dInStr.close();
				currentSock.close();
				
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
}
