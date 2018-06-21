using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommandUI : MonoBehaviour {

    //Manages the Command UI based on the state which are present, absent, out
    

    GameObject commandUI;

	void Start () {
        commandUI = transform.GetChild(0).gameObject;
        commandUI.SetActive(false);
	}
	
	// Update is called once per frame
	void Update () {
        //if (entryMonitor.inHouse)
       // {
         //   CommandManager();
       // }
	}

    void CommandManager()
    {
        if (Input.GetButton("B"))
                commandUI.SetActive(true);

            if (Input.GetButtonUp("B"))
                commandUI.SetActive(false);
    }
}
