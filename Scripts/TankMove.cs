using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TankMove : MonoBehaviour
{
    float tkMvSpeed = 10.0f; // 탱크의 이동 속도
    float tkRotSpeed = 150.0f; // 탱크의 회전 속도


    public int playerNum = 1; // 플레이어 번호
    string mvAxisName; // 이동 축의 이름 저장
    string rotAxisName; // 회전 축의 이름 저장

    void Start()
    {
        mvAxisName = "Vertical" + playerNum; // 플레이어 번호를 사용하여 입력키 부여
        rotAxisName = "Horizontal" + playerNum;
    }

    void Update()
    {
        float mv = Input.GetAxis(mvAxisName) * tkMvSpeed * Time.deltaTime;
        float rot = Input.GetAxis(rotAxisName) * tkRotSpeed * Time.deltaTime;
        transform.Translate(0, 0, mv);
        transform.Rotate(0, rot, 0);
    }
}
