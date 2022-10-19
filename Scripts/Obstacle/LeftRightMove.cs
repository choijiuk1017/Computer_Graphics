using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LeftRightMove : MonoBehaviour
{

    //UD_Floor & LR_Floor
    public bool turnSwitch;
    public float moveSpeed;
    float initPositionX;
    public float distance;
    public float turningPoint;
    // Start is called before the first frame update

    void Awake()
    {
        initPositionX = transform.position.x;
        turningPoint = initPositionX - distance;
    }

    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        leftRight();
    }

    void leftRight()
    {
        float currentPositionX = transform.position.x;

        if (currentPositionX >= initPositionX + distance)
        {
            turnSwitch = false;
        }
        else if (currentPositionX <= turningPoint)
        {
            turnSwitch = true;
        }

        if (turnSwitch)
        {
            transform.position = transform.position + new Vector3(1, 0, 0) * moveSpeed * Time.deltaTime;
        }
        else
        {
            transform.position = transform.position + new Vector3(-1, 0, 0) * moveSpeed * Time.deltaTime;
        }
    }
}
