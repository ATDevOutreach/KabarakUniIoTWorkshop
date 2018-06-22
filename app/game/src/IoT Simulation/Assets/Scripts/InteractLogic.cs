using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class InteractLogic : MonoBehaviour {

    public GameObject[] allGOs = new GameObject[3];
    TextMeshProUGUI interactText, buttonText;

    // Use this for initialization
    void Start()
    {
        interactText = transform.GetChild(1).GetComponent<TextMeshProUGUI>();
        buttonText = transform.GetChild(2).GetComponent<TextMeshProUGUI>();
        disableInteract();
    }

    void enableInteract()
    {
        for (int i = 0; i < allGOs.Length;i++)
        {
            allGOs[i].SetActive(true);
        }
    }

    public void disableInteract()
    {
        for (int i = 0; i < allGOs.Length; i++)
        {
            allGOs[i].SetActive(false);
        }
    }

    public void SetDisplayInteractText(string buttonName, string interactText)
    {
        this.interactText.text = interactText;
        buttonText.text = buttonName;
        enableInteract();
    }
}
