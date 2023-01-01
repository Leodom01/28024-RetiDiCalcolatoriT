package es5;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Serializable;
import java.rmi.Naming;
import java.rmi.RemoteException;

public class ServerImpl extends java.rmi.server.UnicastRemoteObject implements RemOp {

	protected ServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub 
	}

@Override 
public int conta_righe(String fileName, int maxWords) throws RemoteException { 
	System.out.println("Conta righe invocato...");
	System.out.println("file: "+fileName+" words: "+maxWords);
	BufferedReader reader = null; 
	try { 
		try {
			reader = new BufferedReader(new FileReader(fileName)); 
			System.out.println("Aperto reader "+fileName);
		}catch(Exception e) {
			System.out.println("Errore in fase apertura file...");
			e.printStackTrace();
		}
		int validRows = 0; 
		String line; 
	 
		if(!fileName.endsWith(".txt")){ 
			throw new Exception(); 
		} 
		 
		while((line = reader.readLine()) != null) { 
			String[] readWords = line.split(" ");
			int totWords = readWords.length;
			for(int i = 0; i<readWords.length; i++) { 
				if(readWords[i].equals("")) {
						totWords--;
				}
			}
			if(totWords >= maxWords) { 
				validRows++; 
			} 
		}
		System.out.println("Pronto a mandare "+validRows);
		reader.close(); 
		return validRows; 
		}catch(Exception e) { 
			throw new RemoteException(); 
		}finally { 
			try { 
				reader.close(); 
			}catch(Exception e) { 
				System.out.println("Errore in fase chiusura file...");
				throw new RemoteException(); 
			} 
		} 
}

	@Override
	public Serializable[] elimina_riga(String fileName, int rowNum) throws RemoteException {
		System.out.println("Elimina riga invocato...");
		BufferedReader reader = null;
		BufferedWriter writer = null;
		try {
			reader = new BufferedReader(new FileReader(fileName));
			String newFileName = "deleted_" + new Long(System.currentTimeMillis()).toString() + ".txt";
			writer = new BufferedWriter(new FileWriter(newFileName));
			String line;
			int readingLineNum = 0;
			boolean deletedRow = false;

			if (!fileName.endsWith(".txt")) {
				throw new Exception();
			}

			while ((line = reader.readLine()) != null) {
				if (readingLineNum != rowNum) {
					writer.append(line + "\n");
				}else {
					System.out.println("Riga eliminata: "+line);
				}
				readingLineNum++;
			}
			if (readingLineNum <= rowNum) {
				throw new Exception();
			}

			reader.close();
			writer.close();
			Serializable[] toReturn = new Serializable[2];
			toReturn[0] = newFileName;
			toReturn[1] = --readingLineNum;
			
			System.out.println("Pronto a ritornare new filename e file rows...");
			
			return toReturn;
		} catch (Exception e) {
			throw new RemoteException();
		} finally {
			try {
				reader.close();
				writer.close();
			} catch (Exception e) {
				throw new RemoteException();
			}
		}
	}

	public static void main(String args[]) {
		try {
			int regPort = 1099;
			if(args.length == 1) {
				regPort = Integer.parseInt(args[0]);
			}
			
			ServerImpl RMIServer = new ServerImpl();
			Naming.rebind("//localhost:"+regPort+"/RemoteOps", RMIServer);
			
			System.out.println("Server pronto");
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Errore: " + e.getMessage());
			System.exit(1);
		}
	}

}
