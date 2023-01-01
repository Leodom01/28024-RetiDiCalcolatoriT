package es5;

import java.io.Serializable;

public interface RemOp extends java.rmi.Remote{

	/**
	 * Ritorna il numero di righe con numero di parole maggiore di maxWords.
	 * Solleva RemoteException in cao di errore
	 * 
	 */
	public int conta_righe(String fileName, int maxWords) throws java.rmi.RemoteException;
	
	/**
	 * Elimina la riga rowNum dal file fileName. Ritorna Nome file e numero massimo di righe.
	 * Errore se le righe non sono abbastanza o se il file non esiste.
	 * Ritorna [String fileName, int totalRows]
	 * 
	 */
	public Serializable[] elimina_riga(String fileName, int rowNum) throws java.rmi.RemoteException;
	
}
