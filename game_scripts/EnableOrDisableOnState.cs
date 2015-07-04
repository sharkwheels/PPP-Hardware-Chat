using UnityEngine;
using System.Collections;

public class EnableOrDisableOnState : MonoBehaviour {

	public GameState desiredState;

	void Start ()
	{
				if (GameManager.instance.gameState == desiredState)
						gameObject.SetActive (true);
				else
						gameObject.SetActive (false);
				GameManager.instance.OnStateChange += HandleOnStateChange;
	}

	void HandleOnStateChange (GameState state)
	{ 
		if (state == desiredState)
						gameObject.SetActive (true);
	
		else

		gameObject.SetActive (false);
}

void Disable ()

{
	gameObject.SetActive (false);
}
}

