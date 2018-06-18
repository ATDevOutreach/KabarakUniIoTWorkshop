using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerScript : MonoBehaviour {
    Animator anim;
	// Use this for initialization
	void Start () {
        anim = GetComponent<Animator>();
	}
	
	// Update is called once per frame
	void Update () {
        Movement(Input.GetAxis("Horizontal"),Input.GetAxis("Vertical"));
	}

    void Movement(float x, float z)
    {
        transform.rotation = Quaternion.Euler(0f,transform.eulerAngles.y,0f);
        const float V = 0.15f;
        anim.SetFloat("VSpeed", z, V, Time.deltaTime);
        anim.SetFloat("HSpeed", x, V, Time.deltaTime);
    }
}
