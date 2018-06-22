using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainLightChangeLogic : MonoBehaviour {

	// Use this for initialization
	Material lampColor;
	Light lampLight;
	Color myColor, defaultColor;

	void Start () {
		lampColor = GetComponent<Renderer>().material;
		lampLight = GetComponentInChildren<Light>();

		defaultColor = lampLight.color;
		DefaultLightColor();
		Invoke("increaseLightIntensity",21f);
	}
	
	// Update is called once per frame
	void Update () {
		lampColor.SetColor("_EmissionColor",myColor);
		lampLight.color = myColor;
	}

	public void ChangeLightColor(Color newColor)
	{
		IEnumerator e = changeLight(newColor);
		StartCoroutine(e);
	}

	IEnumerator changeLight(Color color)
	{
		float timer = 0f;

		while(timer<4f)
		{
			myColor =  Color.Lerp(myColor,color,2f*Time.deltaTime);
			yield return null;
		}
	}

	public void DefaultLightColor()
	{
		myColor = defaultColor;
	}

	public void increaseLightIntensity()
	{
		StartCoroutine(increaseLight());
	}

	IEnumerator increaseLight()
	{
		float timer = 0f;

		while(timer<4f)
		{
			lampLight.intensity = Mathf.Lerp(lampLight.intensity,7.1f,3*Time.deltaTime);
			timer+=0.1f;
			yield return null;
		}
	}
}
