using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HardBlock : MonoBehaviour
{
    private int count = 0;
    Renderer blockColor;
    public Material softBlock;
    public Material block;
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
        if (col.gameObject.tag == "Ball")
        {
            count++;
            if(count == 1)
            {
                blockColor.material.color = block.color;
            }
            if (count == 2)
            {
                blockColor.material.color = softBlock.color;
            }
            if (count == 3)
            {
                Destroy(gameObject, 0.2f);
            }
        }

    }
}
