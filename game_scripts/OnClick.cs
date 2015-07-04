using UnityEngine;
using System.Collections;

public class OnClick : MonoBehaviour {

	public string levelToLoad;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnMouseDown () {

		Application.LoadLevel (levelToLoad);

		}

}
