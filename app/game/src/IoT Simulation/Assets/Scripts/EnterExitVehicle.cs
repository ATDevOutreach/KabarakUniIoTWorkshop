using System.Collections;
using UnityEngine;

public class EnterExitVehicle : MonoBehaviour {
    CarController carController;
    public CameraFollow cameraFollow;
    public Transform playerFollow, carFollow;
    GameObject player;
    bool isinRange;
    InteractLogic interactUI;

    // Use this for initialization
    void Start () {
        carController = GetComponent<CarController>();
        interactUI = GameObject.FindGameObjectWithTag("Interact").GetComponent<InteractLogic>();
        player = GameObject.FindGameObjectWithTag("Player");
        carController.isInside = false;
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetButtonDown("A"))
        {
            switchInput();
        }

        UIManagement();
	}

    void UIManagement()
    {
        if (isinRange)
        {
            if (carController.isInside)
            {
                interactUI.SetDisplayInteractText("E", "Exit");
            }
            else
            {
                interactUI.SetDisplayInteractText("E", "Enter");
            }
        }
        else
        {
            interactUI.disableInteract();
        }
    }

    void OnTriggerStay(Collider col)
    {
        if (col.CompareTag("Player"))
        {
            //Display UI
            isinRange = true;
        }
    }

    void OnTriggerExit(Collider col)
    {
        if (col.CompareTag("Player"))
        {
            //Remove UI

            isinRange = false;
        }
    }

    void switchInput()
    {
        if (carController.isInside)
        {
            carController.isInside = false;
            player.transform.position = new Vector3(transform.position.x + 4f, transform.position.y, transform.position.z);
            ControlUI();
        }
        else
        {
            if (isinRange)
            {
                carController.isInside = true;
                ControlUI();
            }
        }
    }

    void ControlUI()
    {
        if (carController.isInside)
        {
            //Show driving tips
            //Show press A to exit
            //DisablePlayer
            //Switch FollowTag
            cameraFollow.CameraFollowObj = carFollow;
            player.SetActive(false);

            //if (Input.GetButtonDown("A"))
              //  switchInput();
        }
        else
        {
            //DisableUI
            //Switch FollowTag
            cameraFollow.CameraFollowObj = playerFollow;
            player.SetActive(true);
        }
    }
}
