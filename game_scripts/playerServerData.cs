using UnityEngine;
using System.Collections;

public class playerServerData : MonoBehaviour
{

	// SERVER STUFF
	public bool serverRead = true;

	// array of acceptable moves to comapre the server data to. 

	public string[] moves = new string[3];

	// FROM PLAYER SCRIPT
	public string playerName = "Insert Name";
	private string activeMove;
	private string correctMove;
	
	// Controls attack damage. Right now Move1 is a weak attack, Move2 is a strong attack, and Move3 does nothing.
	public int attackDamage1 = 1;
	public int attackDamage2 = 5;

// HERE WE GOOOO ~~~~~~~~~~~~~~

	void Start()
	{

		socketScript.instance.OnDataReceived += HandleOnDataReceived;
	
	}

	// data handler
	void HandleOnDataReceived (string data)
	{

		// magically getting that variable from sockets
		// "data" is whatever gets put on the server
		// server data is sent either from typing in a client terminal window or, eventually, the bean
		// Debug.Log ("Data from server: " + data);
		data.Trim();


		// MOVE CHECK
		correctMove = GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().moveCheck;
		Debug.Log ("Move that destroys enemy: " + correctMove);

		activeMove = data;

		if (activeMove.StartsWith(correctMove))
		{
			Debug.Log (playerName + " PASSED THE MOVE CHECK");
			activeMove = "";

			if (data.StartsWith(moves[0]))
			{
				GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().currentHealth -= attackDamage1;
				Debug.Log (playerName + " slashed a thing!");
			} 
			
			else if (data.StartsWith(moves[1]))
			{
				GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().currentHealth -= attackDamage2;
				Debug.Log (playerName + " is spinning!");
			}
			
			else if (data.StartsWith(moves[2]))
			{
				// Thrust;
				Debug.Log (playerName + " thrusted a thing!");
			}

		}

		else
		{

			// This is firing as soon as Unity connects to the server and then never happens again. Fix?
			Debug.Log (playerName + " FAILED THE MOVE CHECK");
			activeMove = "";
		}






		Debug.Log (GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().currentHealth);
	}

	void Update ()
	{

	}

	void OnDestroy()
	{
		if(socketScript.instance != null)
			socketScript.instance.OnDataReceived -= HandleOnDataReceived;
	}
	
}

