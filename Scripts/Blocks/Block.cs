using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Block : MonoBehaviour
{
    private int count = 0;
    Renderer blockColor;
    public Material softBlock;

    public GameObject gameManager;
    // Start is called before the first frame update
    void Start()
    {
        blockColor = gameObject.GetComponent<Renderer>();
        gameManager = GameObject.FindWithTag("game");
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision col)
    {
        if(col.gameObject.CompareTag("Ball") || col.gameObject.CompareTag("SecondBall") || col.gameObject.CompareTag("BigBall"))
        {
            count++;
            if(count == 1)
            {
                blockColor.material.color = softBlock.color;
            }
            if(count == 2)
            {
                Destroy(gameObject,0.2f);
                gameManager.GetComponent<GameManger>().blockNum--;

            }
        }

        if (col.gameObject.tag == "SuperBall")
        {
            Destroy(gameObject, 0.2f);
            gameManager.GetComponent<GameManger>().blockNum--;
        }
    }


}
