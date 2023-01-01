package es1;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;

public class DiscoveryServer {

	static ArrayList<String> fileNames = new ArrayList<>();
	static ArrayList<Integer> port = new ArrayList<>();
	static Integer DSPort;
	
	public static void main(String argv[]) {
		if(argv.length < 3) {
			System.err.println("Errore nel numero di parametri");
		}
		
		try {
			DSPort = Integer.parseInt(argv[0]);
			for(int i = 1; i<argv.length; i=i+2) {
				if(!fileNames.contains(argv[i]) && !port.contains(Integer.parseInt(argv[i+1]))) {
					fileNames.add(argv[i]);
					port.add(Integer.parseInt(argv[i+1]));
				}else {
					System.out.println("Filename o port inseriti in modo doppio, ciao ciao...");
					return;
				}
			}
		} catch (NumberFormatException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		for(int i = 0; i<fileNames.size(); i++) {
			System.out.println("Avvio server su porta: "+port.get(i)+" e sul file: "+fileNames.get(i));
			Thread toRun = new RowSwap(port.get(i), fileNames.get(i));
			toRun.start();
		}
		
		//Ora mi metto in attesa e fungo da discovery server
		try {
			DatagramSocket socket = new DatagramSocket(9000);
			byte[] buffer = new byte[1024];
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

			while(true) {
				packet.setData(buffer);
				socket.receive(packet);
				
				ByteArrayInputStream bInStr = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				DataInputStream dInStream = new DataInputStream(bInStr);
				
				String request = dInStream.readUTF();
				System.out.println("Ricevuto: "+request);
				
				ByteArrayOutputStream bOutStr = new ByteArrayOutputStream();
				DataOutputStream dOutStr = new DataOutputStream(bOutStr);
				int indexInNameList = fileNames.indexOf(request);
				if(indexInNameList != -1) {
					dOutStr.writeInt(port.get(indexInNameList));
				}else {
					dOutStr.writeInt(-1);
				}
				byte[] data = bOutStr.toByteArray();
				packet.setData(data);
				socket.send(packet);
				
			}
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
}
