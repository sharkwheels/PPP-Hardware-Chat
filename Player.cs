using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour 
{

	public string playerName = "Player1";
	private string activeMove;
	private string correctMove;

// Controls attack damage. Right now Move1 is a weak attack, Move2 is a strong attack, and Move3 does nothing.
	public int attackDamage1 = 1;
	public int attackDamage2 = 5;

// Move Check stuff
// Unless correctMove = activeMove

// HERE WE GO COMBAT TIME ~~~~~~~~~~~~~~~

	// Use this for initialization
	void Start () 
	{

	}
	
	// Update is called once per frame
	void Update ()
	{

		// Hitch this to a "Monster Is Attacking" conditional to prevent it from clogging the console updates.
		correctMove = GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().moveCheck;
		Debug.Log ("Move that destroys enemy:" + correctMove);

		// Move 1
		if(Input.GetKeyDown (KeyCode.Q))
		{
			activeMove = "Move1";
			GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().currentHealth -= attackDamage1;
			Debug.Log (playerName + activeMove);

		}

		// Move 2
		if(Input.GetKeyDown (KeyCode.W))
		{
			activeMove = "Move2";
			GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().currentHealth -= attackDamage1;
			Debug.Log (playerName + activeMove);
		}

		// Move 3 (Transform)
		if(Input.GetKeyDown (KeyCode.E))
		{
			activeMove = "Move3";
			// Go to Transform State
			Debug.Log (playerName + activeMove);
		}

		// DEBUG
		if(Input.anyKeyDown)
		{
			Debug.Log (GameObject.FindGameObjectWithTag("Enemy").GetComponent<MonsterHealth>().currentHealth);
		}
	}
}