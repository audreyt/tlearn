<?php 

/**
 * @file
 * theming functions for the tlearn module.
 */

function theme_tlearn_admin_troles($variable) {

  $form = $variable['form'];
  
  $rows = array();
  
//  $sss = element_children($form);

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $trole = &$form[$key];

      $row = array();
      $row[] = drupal_render($trole['name']);
      
      if (isset($trole['weight'])) {
        $trole['weight']['#attributes']['class'] = array('tlearn-role-weight');
        $row[] = drupal_render($trole['weight']);
      }
      
      $row[] = drupal_render($trole['terms']); 
      $row[] = drupal_render($trole['edit']);
      $row[] = drupal_render($trole['delete']);
      
      $rows[] = array('data' => $row);
    }
  }

  $header = array(t('Role'));

  $header[] = '閱讀權限'; 
  $header[] = array('data' => '操作', 'colspan' => '2');
  
  return 
    theme('table', array(
      'header' => $header, 
      'rows' => $rows, 
      'empty' => t('尚未建立角色的閱讀權限。 <a href="@link">新增角色閱讀權限</a>.', array('@link' => url('admin/config/tlearn/troles/add'))),
      'attributes' => array('id' => 'tlearn-trole'),
      )
    )
    . drupal_render_children($form);
}





