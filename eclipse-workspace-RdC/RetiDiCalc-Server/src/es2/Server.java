package es2;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

	public static void main(String args[]) {
		
		ServerSocket socket = null;
		String fileName = null;
		File currentDir = new File("./");
		DataInputStream dIStr = null;
		DataOutputStream dOStr = null;
		Socket currsock = null;
		DataOutputStream fileWriter = null;
		
		try {
			socket = new ServerSocket(9000);
		} catch (IOException e) {
			System.err.println("Errore in fase di apertura della socket");
			e.printStackTrace();
			return;
		}
		
		while(true) {
			try {
				currsock = socket.accept();
				System.out.println("Aperuta nuova socket");
			} catch (IOException e) {
				System.err.println("Errore nella connessione con un client");
				e.printStackTrace();
				continue;
			}
			try {
				dIStr = new DataInputStream(currsock.getInputStream());
				dOStr = new DataOutputStream(currsock.getOutputStream());
			} catch (IOException e) {
				System.err.println("Errore durante la apertura degli stream");
				e.printStackTrace();
				continue;
			}
			while(currsock.isConnected()) {
				try {
					System.out.println("Provo ora a leggere fileName");
					fileName = dIStr.readUTF();
					System.out.println("Ricevuto file: "+fileName);
				} catch (IOException e) {
					System.err.println("Errore in fase di lettura del file");
					e.printStackTrace();
					continue;
				}
				System.out.println("Richiesta per file: "+fileName);
				File [] filesInDir = currentDir.listFiles();
				boolean accept = true;
				for(int i = 0; i<filesInDir.length; i++) {
					if(fileName.equals(filesInDir[i].getName())) {
						accept = false;
						break;
					}
				}
				if(!accept) {
					try {
						dOStr.writeUTF("salta");
						continue;
					} catch (IOException e) {
						System.err.println("Errore in fase di scrittura su pipe");
						e.printStackTrace();
						continue;
					}
				}
				try {
					dOStr.writeUTF("attiva");
				} catch (IOException e) {
					System.err.println("Errore in fase di scrittura su pipe");
					e.printStackTrace();
					continue;
				}
				
				File newBorn = new File(fileName);
				long bytesToRead = -1;
				try {
					bytesToRead = dIStr.readLong();
					System.out.println("Ricevuto filesize: "+bytesToRead);
				} catch (IOException e) {
					System.err.println("Errore in fase di lettura da socket");
					e.printStackTrace();
					continue;
				}
				
				try {
					fileWriter = new DataOutputStream(new FileOutputStream(newBorn));
				} catch (FileNotFoundException e) {
					System.err.println("File non trovato");
					e.printStackTrace();
					continue;
				}
				
				try {
					int readBytes = 0; 
					while(readBytes < bytesToRead) {
						byte[] buffer = new byte[4096];
						int thisReading = dIStr.read(buffer);
						fileWriter.write(buffer, 0, thisReading);
						readBytes += thisReading;
					}
				} catch (IOException e) {
					System.err.println("Errore in fase di lettura da socket o scrittura su file");
					e.printStackTrace();
					continue;
				}
				
				try {
					fileWriter.close();
				} catch (IOException e) {
					System.err.println("Errore in fase di chiusura del file");
					e.printStackTrace();
					continue;
				}
				
				System.out.println("Fine scrittura file: "+fileName);
			}
			System.out.println("Fine comunicazione con un client");
		}
		
	}
	
}
