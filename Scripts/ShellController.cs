using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShellController : MonoBehaviour
{
    public ParticleSystem shellExplosion;

    private void OnCollisionEnter(Collision other)
    {
        ParticleSystem fire = Instantiate(shellExplosion, transform.position, Quaternion.identity);
        fire.Play();
        Destroy(gameObject);
        Destroy(fire.gameObject, 2.0f);
    }
}
