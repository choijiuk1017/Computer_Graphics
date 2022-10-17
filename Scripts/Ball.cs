using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball : MonoBehaviour
{
    Vector3 movePos;
    public float speed = 10.0f;
    Rigidbody rigid;
    // Start is called before the first frame update

    void Start()
    {
        rigid = GetComponent<Rigidbody>();

        rigid.AddForce(Vector3.back * 300f);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
