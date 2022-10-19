using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Block : MonoBehaviour
{
    private int count = 0;
    Renderer blockColor;
    public Material softBlock;
    // Start is called before the first frame update
    void Start()
    {
        blockColor = gameObject.GetComponent<Renderer>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision col)
    {
        if(col.gameObject.tag == "Ball")
        {
            count++;
            if(count == 1)
            {
                blockColor.material.color = softBlock.color;
            }
            if(count == 2)
            {
                Destroy(gameObject,0.2f);
            }
        }

    }


}
