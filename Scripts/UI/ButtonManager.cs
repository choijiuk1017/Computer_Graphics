using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class ButtonManager : MonoBehaviour
{
    public Button reStart;
    public Button exit;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OnClickReStart()
    {
        SceneManager.LoadScene("InGame");
    }

    public void OnClickExit()
    {
        Application.Quit();
    }
}
