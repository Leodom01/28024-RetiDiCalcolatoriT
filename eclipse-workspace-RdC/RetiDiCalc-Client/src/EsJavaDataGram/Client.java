package EsJavaDataGram;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class Client {

	public static void main(String args[]) {
		
		try {
			DatagramSocket socket = new DatagramSocket();
			socket.setSoTimeout(30000);
			
			byte[] buffer = new byte[1024];
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, InetAddress.getByName("localhost"), 9000);
			
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
			System.out.println("Inserisci il nome file e riga da leggere separati da spazio, \n^D(Unix)/^Z(Win) invio per uscire");
			String newLine;
			while((newLine = stdIn.readLine()) != null) {
				String file = newLine.split(" ")[0];
				Integer line = Integer.parseInt(newLine.split(" ")[1]);
				String richiesta = file+" "+line.toString();
				
				ByteArrayOutputStream bArrOut = new ByteArrayOutputStream();
				DataOutputStream dOutStream = new DataOutputStream(bArrOut);
				dOutStream.writeUTF(richiesta);
				byte[] toSend = bArrOut.toByteArray();
				
				packet.setData(toSend);
				socket.send(packet);
				
				packet.setData(buffer);
				socket.receive(packet);
				
				ByteArrayInputStream bArrIn = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				DataInputStream dInStream = new DataInputStream(bArrIn);
				String risposta = dInStream.readUTF();
				
				System.out.println("Risposta del server: "+risposta);
				
				System.out.println("Inserisci il nome file e riga da leggere separati da spazio, \n^D(Unix)/^Z(Win) invio per uscire");
			}
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
}
