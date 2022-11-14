using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TankMove : MonoBehaviour
{
    float tkMvSpeed = 10.0f; // ��ũ�� �̵� �ӵ�
    float tkRotSpeed = 150.0f; // ��ũ�� ȸ�� �ӵ�


    public int playerNum = 1; // �÷��̾� ��ȣ
    string mvAxisName; // �̵� ���� �̸� ����
    string rotAxisName; // ȸ�� ���� �̸� ����

    void Start()
    {
        mvAxisName = "Vertical" + playerNum; // �÷��̾� ��ȣ�� ����Ͽ� �Է�Ű �ο�
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