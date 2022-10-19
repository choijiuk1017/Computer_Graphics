using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockSpawn : MonoBehaviour
{
    public GameObject[] prefabs;  

    public int randomNum;

    // Start is called before the first frame update
    void Start()
    {
        randomNum = Random.Range(0, prefabs.Length);
        GameObject block = GameObject.Instantiate(prefabs[randomNum], transform.position, Quaternion.identity);

    }

    // Update is called once per frame
    void Update()
    {
        
    }


}
