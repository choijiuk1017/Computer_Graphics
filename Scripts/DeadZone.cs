using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DeadZone : MonoBehaviour
{
    public GameObject gameManager;
    GameObject ball;
    public bool isball = true;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = GameObject.FindWithTag("game");
    }

    // Update is called once per frame
    void Update()
    {
        ball = GameObject.FindWithTag("Ball");
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Ball"))
        {
            Destroy(ball);
            gameManager.GetComponent<GameManger>().hp -= 1;
            isball = false;
        }
    }
}
