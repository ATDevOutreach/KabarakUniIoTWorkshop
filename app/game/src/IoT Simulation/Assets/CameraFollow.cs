using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour {

	public float CameraMoveSpeed = 120.0f;
	public Transform CameraFollowObj;
	public float clampAngle = 60.0f;
    public float inputSensitivity = 150.0f;
	public float mouseX;
	public float mouseY;
	public float finalInputX;
	public float finalInputZ;
	//public float smoothX;
	//public float smoothY;
	private float rotY = 0.0f;
	private float rotX = 0.0f;
    GameObject activePlayer;

	// Use this for initialization
	void Start () {
        CameraFollowObj = GameObject.FindGameObjectWithTag("Player").transform.GetChild(1).transform;
        activePlayer = GameObject.FindGameObjectWithTag("Player");
		Vector3 rot = transform.localRotation.eulerAngles;
		rotY = rot.y;
		rotX = rot.x;
		Cursor.lockState = CursorLockMode.Locked;
		Cursor.visible = false;
	}
	
	// Update is called once per frame
	void Update () {
		// We setup the rotation of the sticks here
		//float inputX = Input.GetAxis ("RightStickHorizontal");
		//float inputZ = Input.GetAxis ("RightStickVertical");
		mouseX = Input.GetAxis ("Mouse X");
		mouseY = Input.GetAxis ("Mouse Y");
		//finalInputX = inputX + mouseX;
		//finalInputZ = inputZ + mouseY;
        finalInputX = mouseX;
        finalInputZ = mouseY;

        rotY += finalInputX * inputSensitivity * Time.deltaTime;
		rotX += finalInputZ * inputSensitivity * Time.deltaTime;

		rotX = Mathf.Clamp (rotX, -clampAngle, clampAngle);

		Quaternion localRotation = Quaternion.Euler (rotX, rotY, 0.0f);
		transform.rotation = localRotation;

		float moveInput = Input.GetAxis("Vertical");
		if(finalInputX!=0f && moveInput!=0f)
		{
			activePlayer.transform.forward = Vector3.Lerp(activePlayer.transform.forward,transform.forward,3f*Time.deltaTime);
		}
	}

	void LateUpdate () {
		CameraUpdater ();
	}

	void CameraUpdater() {
		// set the target object to follow
		Transform target = CameraFollowObj;

		//move towards the game object that is the target
		float step = CameraMoveSpeed * Time.deltaTime;
		transform.position = Vector3.MoveTowards (transform.position, target.position, step);
	}
}
