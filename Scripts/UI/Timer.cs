using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Timer : MonoBehaviour
{

    public Text gameTimeUI;
    public float setTime = 150;
    int min;
    float sec;

    public GameObject gameoverPanel;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        setTime -= Time.deltaTime;

        // 전체 시간이 60초 보다 클 때
        if (setTime >= 60f)
        {
            // 60으로 나눠서 생기는 몫을 분단위로 변경
            min = (int)setTime / 60;
            // 60으로 나눠서 생기는 나머지를 초단위로 설정
            sec = setTime % 60;
            // UI를 표현해준다
            gameTimeUI.text = "남은 시간 : " + min + "분" + (int)sec + "초";
        }

        // 전체시간이 60초 미만일 때
        if (setTime < 60f)
        {
            // 분 단위는 필요없어지므로 초단위만 남도록 설정
            gameTimeUI.text = "남은 시간 : " + (int)setTime + "초";
        }

        // 남은 시간이 0보다 작아질 때
        if (setTime <= 0)
        {
            // UI 텍스트를 0초로 고정시킴.
            gameTimeUI.text = "남은 시간 : 0초";

            gameoverPanel.SetActive(true);
        }
    }
}
