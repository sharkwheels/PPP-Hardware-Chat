using UnityEngine;
using System.Collections;

public class TextEditor : MonoBehaviour {

	[ExecuteInEditMode]
	[Multiline]	
	public string text;

	GUIText guiText;

	// Use this for initialization
	void Start () {
	
		guiText = GetComponent <GUIText> ();

		guiText.text = text;

	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
