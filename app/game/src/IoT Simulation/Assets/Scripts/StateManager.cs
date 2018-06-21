using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StateManager : MonoBehaviour {
	
	InteractLogic interactUI;
	[HideInInspector]public int states;
	/*
	0 for OUT
	1 for PRESENCE // Can also call the allOff here
	2 for ABSENCE
	3 for KITCHEN

	0 Commands Available - null

	1 Commands Available
	- AllOff
	- Turn On Music
	- Increase Intensity
	- Change Light Color

	2 Commands available - Switch to interact UI
	 - interact:Curtains
	 - interact:Lights
	 */

	 void Start()
	 {
		 interactUI = GameObject.FindGameObjectWithTag("Interact").GetComponent<InteractLogic>();
	 }

	 void Update(){updateState();}

	 void updateState()
	 {
		 if(states==0)
		 interactUI.SetDisplayInteractText(states.ToString(),"Out");
		 else if(states==1)
		 interactUI.SetDisplayInteractText(states.ToString(),"MainRoom");
		 else if(states==2)
		 interactUI.SetDisplayInteractText(states.ToString(),"Bedroom");
		  else if(states==3)
		 interactUI.SetDisplayInteractText(states.ToString(),"Kitchen");
	 }
}
