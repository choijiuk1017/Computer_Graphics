using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoftBlock : MonoBehaviour
{
    private int count;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.tag == "Ball")
        {
            count++;
            if (count == 1)
            {
                Destroy(gameObject,0.2f);
            }
        }
    }
}
