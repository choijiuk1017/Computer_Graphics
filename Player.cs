using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Player : MonoBehaviour
{
    public float moveSpeed = 5f;
    public float rotationSpeed = 360f;
    public float jumpPower;
    public GameObject image;

    public int dot = 3;
    CharacterController characterController;

    Animator anim;

    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();

        anim = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 direction = new Vector3(Input.GetAxis("Horizontal"), 0, Input.GetAxis("Vertical"));

        if(direction.sqrMagnitude > 0.01f)
        {
            Vector3 forward = Vector3.Slerp(
                transform.forward,
                direction,
                rotationSpeed * Time.deltaTime / Vector3.Angle(transform.forward, direction));
            transform.LookAt(transform.position + forward);
        }

        if(Input.GetKeyDown(KeyCode.Space))
        {
            direction.y = jumpPower;
        }

        characterController.Move(direction * moveSpeed * Time.deltaTime);

        anim.SetFloat("Speed", characterController.velocity.magnitude);

        if (dot == 0)
        {
            SceneManager.LoadScene("Success");
        }
    }


    private void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.CompareTag("Dot"))
        {
            dot--;
        }

        if(other.gameObject.CompareTag("Enemy"))
        {
            Destroy(gameObject);
            Time.timeScale = 0;
            image.SetActive(true);
        }
    }

}
