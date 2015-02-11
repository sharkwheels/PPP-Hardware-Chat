using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class StoryArray : MonoBehaviour 
	
{
	
	[Multiline]
	public string[] 
	stories;
	
	// Added this so we can tell the state machine where to go next
	
	public GameState desiredState;
	Text uiText;
	int index;
	
	// Use this for initialization
	void Start () 
	{
		index = 0;
		uiText = GetComponent<Text> ();
		uiText.text = stories [index];
	}
	
	// Update is called once per frame
	void Update () 
	{
		
		//If we hit a key and we are not at the end of the array, advance text to next array spot
		if (Input.GetKeyDown (KeyCode.Space) && index < stories.Length) 
		{
			//increase index to next spot
			index++;
			//set text to next spot in array
			uiText.text = stories [index];
		}
		
		else if (Input.GetKeyDown (KeyCode.Space)) 
		{
			//reset index
			index = 0;
			//change to the state after this story sequence
			GameManager.instance.SetGameState (desiredState);
		}
		
		
	}
}