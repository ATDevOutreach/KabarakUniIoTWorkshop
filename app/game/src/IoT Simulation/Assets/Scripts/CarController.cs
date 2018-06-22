using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarController : MonoBehaviour {

    public bool isInside;
    Rigidbody rb;
    float aSpeed;
    public GameObject[] tyres = new GameObject[4];

	// Use this for initialization
	void Start () {
        rb = GetComponent<Rigidbody>();
	}

    void tyreMover()
    {
        for (int i=0;i<tyres.Length;i++)
        {
            tyres[i].transform.rotation = Quaternion.Euler(aSpeed*Time.deltaTime,0f,0f);
        }
    }
	
	// Update is called once per frame
	void FixedUpdate () {

        if (isInside)
        {
            float turnInput = Input.GetAxis("Horizontal");
            float moveInput = Input.GetAxis("Vertical");
            float controllerMoveInput = Input.GetAxis("Triggers");

            DriveKeyboard(turnInput,moveInput);
            DriveController(turnInput,controllerMoveInput);
        }
	}

    void DriveKeyboard(float x,float z)
    {
        float maxValue = 0f;
        if (z > 0.1f)
        {
            maxValue = 7f;
            aSpeed += 0.2f;
            rb.velocity = transform.right * aSpeed*1f;
        }
        else if (z < -0.1f)
        {
            maxValue = 3f;
            aSpeed += 0.1f;
            rb.velocity = transform.right * aSpeed*1f;
        }
        else if (z == 0f)
        {
            aSpeed -= 0.01f;
        }

        if (aSpeed>0f)
        {
            transform.Rotate(0f, 0f, x * 90 * Time.deltaTime);
        }

        if (aSpeed>maxValue)
        {
            aSpeed = maxValue;
        }
        if (aSpeed < 50f)
        {
            aSpeed = 50f;
        }
        
    }

    void DriveController(float x, float z)
    {
        float maxValue = 0f;
        if (z > 0.1f)
        {
            maxValue = 210f;
            aSpeed += 2f;
            rb.AddForce(transform.right * aSpeed * 10f);
        }
        else if (z < -0.1f)
        {
            maxValue = 100f;
            aSpeed += 1f;
            rb.AddForce(-transform.right * aSpeed * 20f);
        }
        else if (z == 0f)
        {
            aSpeed -= 0.01f;
        }

        if (aSpeed > 0f)
        {
            transform.Rotate(0f, 0f, x * 90 * Time.deltaTime);
        }

        if (aSpeed > maxValue)
        {
            aSpeed = maxValue;
        }
        if (aSpeed < 50f)
        {
            aSpeed = 50f;
        }

    }
}
