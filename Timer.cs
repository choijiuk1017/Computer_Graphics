using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Timer : MonoBehaviour
{
    float time = 10f;

    public Text timeText;

    public GameObject image;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        time -= Time.deltaTime;
        timeText.text = "TIMER: " + time;

        if(time == 0)
        {
            Time.timeScale = 0;
            image.SetActive(true);
        }
    }
}
