using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PresenceMonitor : MonoBehaviour {

	// Use this for initialization
	[Range(0,2)]
	public int sensorType;
	/*
	O sensorTypes detect for if the player is in the house or out 
	1 sensorTypes detect for if the player is in the mainroom or out
	2 sensorTypes detect for if the player is in the bedroom or out
	*/

	StateManager stateManager;

	void Start () {
		stateManager = GameObject.FindGameObjectWithTag("GameController").GetComponent<StateManager>();
	}

	//if the sensor type is 0, then we are watching its exit else we are watching its entry

	void OnTriggerStay(Collider col)
	{
		if(col.CompareTag("Player"))
		{
			if(sensorType==0)
			stateManager.states = 3;
			else
			stateManager.states = sensorType;
		}
		
	}

	void OnTriggerExit(Collider col)
	{
		if(col.CompareTag("Player"))
		{
			if(sensorType==0)
			stateManager.states = sensorType;
			else
			stateManager.states = 0;
		}
	}
}
