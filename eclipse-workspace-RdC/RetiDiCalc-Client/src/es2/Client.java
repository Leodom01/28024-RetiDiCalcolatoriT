package es2;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {

	public static void main(String args[]) {
		
		Socket socket = null;
		DataInputStream dIStr = null;
		DataOutputStream dOStr = null;
		String dirToMove = null;
		int SERVER_PORT = 9000;
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		DataInputStream fileReader = null;
		
		try {
			socket = new Socket(InetAddress.getByName("localhost"), SERVER_PORT);
		} catch (UnknownHostException e) {
			System.err.println("Host non trovato.");
			e.printStackTrace();
			return;
		} catch (IOException e) {
			System.err.println("Errore in fase aperuta socket.");
			e.printStackTrace();
			return;
		}
		try {
			dOStr = new DataOutputStream(socket.getOutputStream());
			dIStr = new DataInputStream(socket.getInputStream());
		} catch (IOException e) {
			System.err.println("Errore in fase apertura stream.");
			e.printStackTrace();
			return;
		}
		
		System.out.println("Inserisci il path della directory da trasferire:");
		try {
			while((dirToMove = stdIn.readLine()) != null) {
				if((new File(dirToMove)).isDirectory()) {
					for(File temp : new File(dirToMove).listFiles()) {
						if(temp.isFile()) {
							System.out.println("Inizio a trasferire "+temp.getName());
							dOStr.writeUTF(temp.getName());
							long fileLen = temp.length();
							String response = dIStr.readUTF();
							if(response.equals("attiva")) {
								dOStr.writeLong(fileLen);
								System.out.println("Inviato il nome file, parte scambio: "+temp.getName());
								fileReader = new DataInputStream(new FileInputStream(temp));
								byte[] buffer = new byte[4096];
								int readBytes;
								while((readBytes = fileReader.read(buffer)) != -1) {
									System.out.println("Invio "+readBytes+" bytes");
									dOStr.write(buffer, 0, readBytes);
								}
								fileReader.close();
								System.out.println("Finita la lettura di un file");
							}else {
								System.out.println("Il file non è stato accettato.");
							}
						}
					}
					System.out.println("Ho scritto tutti i file nel folder");
				}else {
					System.out.println("La dir inserita non è una directory, riprova");
				}
				
				System.out.println("Inserisci il path della directory da trasferire:");
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		System.out.println("Finito tutto, ora chiudo connessioni e me ne vo...");
		try {
			stdIn.close();
			socket.close();
		} catch (IOException e) {
			System.err.println("Errore in fase di chiusura di socket o di System in");
			e.printStackTrace();
			return;
		}
		
	}
	
}
