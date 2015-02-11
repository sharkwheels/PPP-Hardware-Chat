using UnityEngine;
using System.Collections;

public class Timer : MonoBehaviour
{
	
	public GameState nextState;
	public float duration;
	float origDuration;
	
	
	// Use this for initialization
	void Start ()
	{
		origDuration = duration;
	}
	
	// Update is called once per frame
	void Update ()
	{
		if (duration >= 0) {
			duration -= Time.deltaTime;
			
		} else {
			duration = origDuration;
			GameManager.instance.SetGameState (nextState);
		}
	}
}