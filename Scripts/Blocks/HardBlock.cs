using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HardBlock : MonoBehaviour
{
    private int count = 0;
    Renderer blockColor;
    public Material softBlock;
    public Material block;

    public GameObject[] items;

    public int randomNum;

    public GameObject gameManager;
    // Start is called before the first frame update
    void Start()
    {
        blockColor = gameObject.GetComponent<Renderer>();
        randomNum = Random.Range(0, items.Length);
        gameManager = GameObject.FindWithTag("game");
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.CompareTag("Ball") || col.gameObject.CompareTag("SecondBall") || col.gameObject.CompareTag("BigBall"))
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
                gameManager.GetComponent<GameManger>().blockNum--;
                GameObject item = GameObject.Instantiate(items[randomNum], transform.position, Quaternion.Euler(0f,0f,90f));
            }
        }

        if (col.gameObject.tag == "SuperBall")
        {
            Destroy(gameObject, 0.2f);
            gameManager.GetComponent<GameManger>().blockNum--;
            GameObject item = GameObject.Instantiate(items[randomNum], transform.position, Quaternion.Euler(0f, 0f, 90f));
        }

    }
}
