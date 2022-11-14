using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TankShoot : MonoBehaviour
{
    public Rigidbody prefabShell;
    public Transform fireTransform;

    public int playerNum = 1; // 플레이어 번호
    string fireName;

    void Start()
    {
        fireName = "Fire" + playerNum;
    }

    void Update()
    {
        if (Input.GetButtonDown(fireName))
        {
            Fire();
        }
    }

    void Fire()
    {
        Rigidbody shellInstance = Instantiate(prefabShell, fireTransform.position,
        fireTransform.rotation) as Rigidbody;
        // Shell에 속도 부여: 탱크의 정면 방향의 힘으로 설정
        shellInstance.velocity = 20.0f * fireTransform.forward;
    }
}
