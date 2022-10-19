using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SecondBall : MonoBehaviour
{
    public float speed;

    public Vector3 direction = new Vector3(1, 0, 1);

    public int count;

    // Start is called before the first frame update
    void Start()
    {
        transform.forward = direction.normalized;

        count = 10;
    }

    // Update is called once per frame
    void Update()
    {
        transform.position += transform.forward * Time.deltaTime * speed;
    }

    private void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.CompareTag("Wall"))
        {
            direction = Vector3.Reflect(direction, col.GetContact(0).normal);
            transform.forward = direction.normalized;

            count--;
        }
        if (count == 0)
        {
            Destroy(gameObject);
        }
    }
}
