using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TankDamage : MonoBehaviour
{
    public ParticleSystem targetExplosion;
    public GameObject bustedTank;

    public int life = 2;
    private void OnCollisionEnter(Collision coll)
    {
        if (coll.collider.tag == "SHELL")
        {
            life--;
        }
        if (life == 0)
        {
            ParticleSystem fire = Instantiate(targetExplosion, transform.position, Quaternion.identity);
            fire.Play();
            Destroy(gameObject);
            Destroy(fire.gameObject, 2.0f);

            GameObject bust = Instantiate(bustedTank, transform.position, Quaternion.identity);
            Destroy(bust.gameObject, 3.0f);
        }
    }
}
