using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Enemy : MonoBehaviour
{
    NavMeshAgent agent;
    GameObject target;

    Animator anim;

    Rigidbody rigid;

    public bool isWalk;

    // Start is called before the first frame update
    void Start()
    {
        agent = GetComponent<NavMeshAgent>();
        target = GameObject.Find("Player");

        anim = GetComponent<Animator>();

        rigid = GetComponent<Rigidbody>();


        isWalk = true;

        
    }

    // Update is called once per frame
    void Update()
    {
        if(isWalk == true)
        {
            agent.destination = target.transform.position;
            agent.speed = 3.5f;
            anim.SetBool("isWalk", true);
        }
        else
        {
            agent.speed = 0;
            anim.SetBool("isWalk", false);
        }
       

        
    }

    void returnWalk()
    {
        isWalk = true;
        agent.destination = target.transform.position;
    }

    private void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.CompareTag("Dot"))
        { 
            isWalk = false;
            Invoke("returnWalk", 3f);
        }
    }
}
