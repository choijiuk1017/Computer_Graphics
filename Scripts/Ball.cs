using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Ball : MonoBehaviour
{ 
    public float speed;

    public Vector3 direction = new Vector3(1, 0, 1);

    public GameObject spawnPoint;

    public GameObject ball;

    public bool isball = true;

    Renderer ballColor;

    public Material superBall;

    public Material normalBall;

    public GameObject timer;
    // Start is called before the first frame update

    void Start()
    {
        transform.forward = direction.normalized;
        ballColor = gameObject.GetComponent<Renderer>();
        timer = GameObject.FindWithTag("Timer");
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        transform.position += transform.forward * Time.deltaTime * speed;
    }

    void returnNormal()
    {
        ballColor.material.color = normalBall.color;

        gameObject.tag = "Ball";

        gameObject.transform.localScale = new Vector3(1, 1, 1);
    }

    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.CompareTag("Wall"))
        {
            direction = Vector3.Reflect(direction, col.GetContact(0).normal);
            transform.forward = direction.normalized;
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.CompareTag("SuperBall") && gameObject.tag == "Ball")
        {
            ballColor.material.color = superBall.color;
            gameObject.tag = "SuperBall";
            Invoke("returnNormal", 3f);
        }

        if(other.gameObject.CompareTag("BigBall") && gameObject.tag == "Ball")
        {
            gameObject.transform.localScale = new Vector3(1.5f, 1.5f, 1.5f);
            gameObject.tag = "BigBall";
            Invoke("returnNormal", 5f);
        }

        if (other.gameObject.CompareTag("SecondBall"))
        {
            GameObject newball = Instantiate(ball, gameObject.transform.localPosition, Quaternion.identity);
        }

        if(other.gameObject.CompareTag("AddTime"))
        {
            timer.GetComponent<Timer>().setTime += 10;
        }
    }

}
