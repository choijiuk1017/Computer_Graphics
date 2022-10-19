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
    // Start is called before the first frame update

    void Start()
    {
        transform.forward = direction.normalized;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        transform.position += transform.forward * Time.deltaTime * speed;
    }

    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.CompareTag("Wall"))
        {
            direction = Vector3.Reflect(direction, col.GetContact(0).normal);
            transform.forward = direction.normalized;
        }
    }



}
