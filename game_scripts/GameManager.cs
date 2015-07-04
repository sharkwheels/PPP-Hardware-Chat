using UnityEngine;
using System.Collections;
using System.Collections.Generic;
//using UnitySampleAssets._2D;
using UnityEngine.UI;

public enum GameState
{
	TitleScreen,
	AcquireSceptre,
	TransformSceptre,
	AcquireWeaponX,
	TransformWeaponX,
	StoryPrologue,
	BattleTutorial,
	BattleVictory,
	BattleFail,
	StoryBody,
	BattleGrunt,
	StoryClimax,
	Transformation,
	BattleBoss,
	StoryEnd,
	Credits

}

// INPUT KEY
// if (Input.GetKeyDown("space"))
// Change "space" to a variable string so we can update the key thru whole doc


public class GameManager : MonoBehaviour {

	// Use this for initialization
	void Awake () {

		OnStateChange += HandleOnStateChange;
		SetGameState (GameState.TitleScreen);
	
	}

	public System.Action<GameState> OnComplete;

	public GameState gameState { get; private set; }
	public GameState nextState;
	public delegate void OnStateChangeHandler (GameState nextState);
	public event OnStateChangeHandler OnStateChange;

	delegate void OnStateUpdate ();
	OnStateUpdate onStateUpdateHandler;

	public void SetGameState (GameState gameState)
	{
				this.gameState = gameState;
				if (OnStateChange != null) {
						OnComplete = null;
						OnStateChange (gameState);
						Debug.Log (gameState);
				}
		}

	IEnumerator DelayedSetState (float duration, GameState state)
	{
		yield return new WaitForSeconds (duration);
		SetGameState (state);

	}


	public void HandleOnStateChange (GameState state)
	{
		onStateUpdateHandler = null;
		if (gameState == GameState.TitleScreen) 
		{
			//replace with sceptre button code
			onStateUpdateHandler = () => {
				if (Input.GetKeyDown("space"))
				SetGameState (GameState.AcquireSceptre);

			}; 

	} else if (gameState == GameState.AcquireSceptre) {

			onStateUpdateHandler = () => {
				if (Input.GetKeyDown("space"))
				SetGameState (GameState.TransformSceptre);
				
			}; 
		
		
	} else if (gameState == GameState.TransformSceptre) {

			onStateUpdateHandler = () => 
			{
				if (Input.GetKeyDown("space"))
					// THIS SCENE SKIPS TO BATTLEGRUNT- CHANGE IN EXPANDED VERSION
					SetGameState (GameState.BattleGrunt);
			}; 
		
			
	} else if (gameState == GameState.AcquireWeaponX) {

			
	} else if (gameState == GameState.TransformWeaponX) {

	
	} else if (gameState == GameState.StoryPrologue) {

			
	} else if (gameState == GameState.BattleTutorial) {

			
	} else if (gameState == GameState.StoryBody) {

			
	} else if (gameState == GameState.BattleGrunt) {

			// COMBAT MODE SHOULD BE A SEPARATE FUNCTION CALLED HERE

			//

			//onStateUpdateHandler = () => 
			//{
			
			//	if (GameObject.Find("TestMonster").GetComponent<MonsterHealth>().enemyDestroyed) //will check if true
			//	Debug.Log ("Victory!");
			//	SetGameState (GameState.BattleVictory);

			//};

	

	} else if (gameState == GameState.BattleVictory) {

	} else if (gameState == GameState.BattleFail) {

	} else if (gameState == GameState.StoryClimax) {

			
	} else if (gameState == GameState.Transformation) {

			
	} else if (gameState == GameState.BattleBoss) {

			
	} else if (gameState == GameState.StoryEnd) {


	} else if (gameState == GameState.Credits) {
			
		}
		
	}


	// Update is called once per frame
	void Update ()
	{
		if (Input.GetKeyDown (KeyCode.R))
			ResetGame ();
		if (Input.GetKeyDown (KeyCode.Escape))
			Application.Quit ();
		//This gets called every frame and can have things added and removed from it
		if (onStateUpdateHandler != null)
			onStateUpdateHandler ();
	}

	public void StartGame ()
	{
		SetGameState (GameState.AcquireSceptre);

	}

	public void ResetGame ()	
	{
		SetGameState (GameState.TitleScreen);
		Application.LoadLevel (Application.loadedLevel);
	}

	public void EndGame ()
	{
		SetGameState (GameState.Credits);
	}

	static GameManager _instance = null;
	public static GameManager instance {
		get {
			if (!_instance) {
				_instance = FindObjectOfType (typeof(GameManager)) as GameManager;
				if (!_instance) {
					var obj = new GameObject ("GameManager");
					_instance = obj.AddComponent<GameManager> ();
				}
			}

			return _instance;

		}
	}

	void OnApplicationQuit ()
	{
		// release reference on exit
		_instance = null;
	}

}
		           