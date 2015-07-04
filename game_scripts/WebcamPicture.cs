using UnityEngine;
using System.Collections;

public class WebcamPicture : MonoBehaviour {

	WebCamTexture webcamTexture;
	public string deviceName;
	public static WebcamPicture Instance { get; private set; }
	// Use this for initialization
	void Start ()
	{
		Instance = this;
		WebCamDevice[] devices = WebCamTexture.devices;
		deviceName = devices [0].name;
		webcamTexture = new WebCamTexture (deviceName, 400, 100, 12);
		renderer.material.mainTexture = webcamTexture;
		webcamTexture.Play ();
	}
	public Texture2D heightMap;
	public Vector3 size = new Vector3 (100, 10, 100);
	private string savePath = "Users/aliciacontestabile/Documents/015/Game Jams/Feb Fatale/MagicalGirlPictures/";
	int captureCounter = 0;

	public Texture2D TakeSnapshot ()
	{

		Texture2D snap = new Texture2D (webcamTexture.width, webcamTexture.height);
		snap.SetPixels (webcamTexture.GetPixels ());
		snap.Apply ();
		// System.IO.File.WriteAllBytes (savePath + captureCounter.ToString () + ".png", snap.EncodeToPNG ());
		captureCounter++;
		return snap;
	}

	// Update is called once per frame
	void Update ()
	{

	}

}
