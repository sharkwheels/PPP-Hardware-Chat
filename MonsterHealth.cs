using UnityEngine;
using System.Collections;

public class MonsterHealth : MonoBehaviour
{

	
	//private bool correctPose = false;
	//If correctPose does not = true when timer <= 0, goto FAIL

	// ENEMY HEALTH
	public string enemyName = "Horace"; 
	// public float damageAmount = 1.0f;
	public float maxHealth = 3.0f;
	private bool enemyDestroyed = false;	
	// Only public so it can be accessed by Player class.
	public float currentHealth;

	// MOVE CHECK  (PUBLIC FOR PLAYER CLASS)
	public string moveCheck = "Move 1-3";
	//public bool isAttacking = false;

	// TIMER
	public GameState victoryState;
	public GameState failState;
	public float duration;
	float origDuration;

	void Start ()
	{
		origDuration = duration;
		currentHealth = maxHealth;

		//isAttacking = true;
		Debug.Log ("Enemy Is Attacking!");
	}

	// Change OnMouseDown to MoveCheck from GameManager- Click is for debug only
	void OnMouseDown ()
	{
		// 
		// currentHealth -= damageAmount;
		// Debug.Log (currentHealth);
	}

	// Update is called once per frame
	void Update ()
	{

		if (currentHealth <= 0)
		{
			enemyDestroyed = true;
			//isAttacking = false;
			Debug.Log ("Enemy DESTROYED");
			GameManager.instance.SetGameState (victoryState);
		}


		if (duration >= 0)
		{
			duration -= Time.deltaTime;
			
		} 

		else
		{
			duration = origDuration;
			Debug.Log ("You FAILED");
			GameManager.instance.SetGameState (failState);
		}
	}
}
