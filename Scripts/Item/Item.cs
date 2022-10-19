using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Item : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.CompareTag("Ball") || other.gameObject.CompareTag("SuperBall") || other.gameObject.CompareTag("BigBall"))
        {
            Destroy(gameObject);
        }
    }
}
