<?php 

/**
 * @file
 * Admin page callbacks for the tlearn module.
 */

function _tlearn_check_private_folder() {
  $private_path = variable_get('file_private_path'); 
  if (empty($private_path)) {
    drupal_set_message(
        format_string('在使用 Tlearn 模組之前，您必需先啟動 Drupal 的 !link', 
            array('!link' => l('私人目錄', 'admin/config/media/file-system'))), 
        'warning'); 
  }
}


/*
 * 課程參數設定
 */

/* 
 * @todo 要不要增加 cloudfront 設定測試的功能？ 
 */
function tlearn_admin_tlearn($form, &$form_state) {
  $form = array();
  
  $form['markup0'] = array(
    '#markup' => '請先閱讀本頁各項設定細節，在準備完成後，再進行設定。',
  ); 
  
  $form['tlearn_default_options'] = array(
    '#type' => 'fieldset',
    '#title' => '課程基本設定',
    '#collapsible' => FALSE,
    '#description' => 'Tlearn 課程基本設定，一律必填。',
  );  
  
  $form['tlearn_default_options']['tlearn_class_type'] = array(
    '#type' => 'textfield',
    '#title' => '課程內容類型', 
    '#required' => true, 
    '#description' => '請選擇課程所使用的內容類型機器名稱, 可指定多個, 利用半形逗點隔開。',
    '#default_value' => variable_get('tlearn_class_type', null),    
  );

  $form['tlearn_default_options']['tlearn_class_vocabulary'] = array(
    '#type' => 'select',
    '#title' => '課程類別使用的 Vocabulary',
    '#required' => true, 
    '#options' => tlearn_vocabulary_options(),
    '#description' => '請選擇課程類別的 Vocabulary (請先建立此 Vocabulary)。',
    '#default_value' => variable_get('tlearn_class_vocabulary', null),
  );
  
  $form['tlearn_default_options']['tlearn_class_termfield'] = array(
    '#type' => 'select',
    '#title' => '課程類別 term 欄位', 
    '#required' => true, 
    '#options' => tlearn_field_options(),    
    '#description' => '請填寫課程內容類型中，term 欄位',
    '#default_value' => variable_get('tlearn_class_termfield', null),    
  );

  $form['tlearn_default_options']['tlearn_class_nofield'] = array(
    '#type' => 'select',
    '#title' => '課程編號欄位', 
    '#required' => true, 
    '#options' => tlearn_field_options(),    
    '#description' => '請填寫課程內容類型中，課程編號欄位',
    '#default_value' => variable_get('tlearn_class_nofield', null),    
  );

  $description = 
  '如果您要銷售單一課程，需指定購買後課程有效天數。可輸入例如 1 days, 10 days, 1 months, 3 months, 1 years 等等。此欄位可以作成 list(text) 類型的欄位，方便您的輸入。'; 
  $form['tlearn_default_options']['tlearn_class_durationfield'] = array(
    '#type' => 'select',
    '#title' => '購買時，課程有效天數欄位', 
    '#options' => tlearn_field_options(),  
    '#empty_value' => 0, 
    '#description' => $description,
    '#default_value' => variable_get('tlearn_class_durationfield', null),    
  );
  
  $description = 
  '如果課程附帶有下載檔案，並且要依據課程權限提供下載，請填寫課程的下載欄位。<span style="color:red;">注意：檔案存放目錄必需是在 private://tlearn/ 底下。</span>';
  $form['tlearn_default_options']['tlearn_class_downloadfield'] = array(
    '#type' => 'select',
    '#title' => '課程下載檔案欄位', 
    '#options' => tlearn_field_options(),  
    '#empty_value' => 0, 
    '#description' => $description,
    '#default_value' => variable_get('tlearn_class_downloadfield', null),    
  );
  
  $description = '如果您準備利用 Drupal 網站內的某個目錄存放課程影片作為播放路徑之一，請填寫本區資料。路徑必需是在 private://tlearn 底下。';
  $form['tlearn_site_folder'] = array(
    '#type' => 'fieldset',
    '#title' => '課程影片 Site 路徑設定',
    '#collapsible' => FALSE,
    '#description' => $description, 
  );  
  
  // @todo 路徑的處理，參考 ubercart credit card 的方法 
  $form['tlearn_site_folder']['tlearn_site_path'] = array(
    '#type' => 'textfield',
    '#title' => 'drupal site 內課程影片目錄', 
    '#description' => '請填寫影片放在 site 中的路徑，例如路徑是 private://tlearn/class/videos 時，請填寫 class/videos，前後不要再加斜線。', 
    '#default_value' => variable_get('tlearn_site_path', null),        
    );

  
  $description = '如果您準備利用 Amazon AWS 的 Cloudfront 作為影片播放方法之一，請填寫本區資料。';
  $form['tlearn_setting_cloudfront'] = array(
    '#type' => 'fieldset',
    '#title' => '課程影片 Amazon Cloudfront 設定',
    '#collapsible' => FALSE,
    '#description' => $description, 
  );  
  $form['tlearn_setting_cloudfront']['tlearn_cloudfront_domain'] = array(
    '#type' => 'textfield',
    '#title' => 'AWS Cloudfront Domain', 
    '#description' => '請填寫您的 cloudfront 的 domain name (不含 http 字頭，不含最後的斜線)', 
    '#default_value' => variable_get('tlearn_cloudfront_domain', null),        
  );
  $form['tlearn_setting_cloudfront']['tlearn_cloudfront_key_filename'] = array(
    '#type' => 'textfield',
    '#title' => 'AWS Cloudfront Key File', 
    '#description' => '請填寫您的 key file 放置的完整路徑（含檔名）。此路徑請置於 Apache 無法存取的地方。' . 
        '不要放在 Drupal 網站裡。' .
        'Key File 請參考 <a href="http://docs.amazonwebservices.com/AWSSecurityCredentials/1.0/AboutAWSCredentials.html#KeyPairs">Amazon CloudFront Key Pairs</a>.',
    '#default_value' => variable_get('tlearn_cloudfront_key_filename', null),        
  );
  $form['tlearn_setting_cloudfront']['tlearn_cloudfront_pair_id'] = array(
    '#type' => 'textfield',
    '#title' => 'AWS Cloudfront Key Pair id', 
    '#description' => '請填寫您的 Cloudfront Key Pair id',
    '#default_value' => variable_get('tlearn_cloudfront_pair_id', null),        
  );
  $form['tlearn_setting_cloudfront']['tlearn_cloudfront_prefixdir'] = array(
    '#type' => 'textfield',
    '#title' => 'AWS Cloudfront 前置目錄', 
    '#description' => '請填寫在 Cloudfront 中影片檔的前置目錄。例如，如果影片檔名的位置是在 "dir1/dir2/13001/00-01.mp4"，其中的 "13001" 是課程編號。那麼請填寫 "dir1/dir2"，後面不要加上斜線。',
    '#default_value' => variable_get('tlearn_cloudfront_prefixdir', null),        
  );
  
  
  $description = '課程影片置放在 Youtube 時的設定。 本區無需進行任何設定。';
  $form['tlearn_setting_youtube'] = array(
    '#type' => 'fieldset',
    '#title' => '課程影片 Youtube 設定',
    '#collapsible' => FALSE,
    '#description' => $description, 
  );  

  return system_settings_form($form);
}


/*
 * 檢查重點：
 * 
 * 課程 term 欄位(*)必需存在於課程內容類型中 
 * 課程 term 欄位(*)必需使用課程類別 vocabulary 
 * 
 * site path (opt) 必需存在，並且無 apache 公開存取權限
 * 
 */
function tlearn_admin_tlearn_validate($form, &$form_state) {
  $error = false; 
  
  /* 
   * 課程內容類型檢查: tlearn_class_type
   */
  $class_types = array_map('trim', explode(',', $form_state['values']['tlearn_class_type'])); 
  
  if (!$class_types || empty($class_types)) {
    form_set_error('tlearn_class_type', '課程內容類型設定錯誤，請確認每一個輸入的內容類型機器名稱'); 
    return; 
  }

  // 檢查每一個 type name 都是正確的
  $node_types = array_keys(node_type_get_names());  // keyd by type

  foreach($class_types as $type) {
    if (!in_array($type, $node_types)) {
      form_set_error('tlearn_class_type', 
          format_string('"%type" 不是內容類型', array('%type'=> $type))); 
      $error = true; 
    }
  }
  if ($error) return; 

  
  /*
   * 課程類別使用的 Vocabulary: tlearn_class_vocabulary
   * 毋需檢查
   */
  
  /* 
   * 課程類別 term 欄位: tlearn_class_termfield
   */
  $term_field = $form_state['values']['tlearn_class_termfield']; 
  
  foreach($class_types as $bundle) {
    $bundle_fields = array_keys(field_info_instances('node', $bundle));
    
    if (!in_array($term_field, $bundle_fields)) {
      form_set_error('tlearn_class_termfield', 
        format_string('內容類型: @type 中無此 term 欄位: @field', 
            array('@type' => $bundle, '@field'=>$term_field)));
      
      $error = true; 
    }
  }
  
  /* 
   * 課程編號欄位: tlearn_class_nofield
   */
  $classno_field = $form_state['values']['tlearn_class_nofield']; 
  
  foreach($class_types as $bundle) {
    $bundle_fields = array_keys(field_info_instances('node', $bundle));
    
    if (!in_array($classno_field, $bundle_fields)) {
      form_set_error('tlearn_class_nofield', 
        format_string('內容類型: @type 中無此課程編號欄位: @field', 
            array('@type' => $bundle, '@field'=>$classno_field)));
      
      $error = true; 
    }
  }
  
  /* 
   * 購買時，課程有效天數欄位: tlearn_class_durationfield
   * 有填寫時，才檢查
   */
  $duration_field = $form_state['values']['tlearn_class_durationfield']; 
  if (!empty($duration_field)) {
    foreach($class_types as $bundle) {
      $bundle_fields = array_keys(field_info_instances('node', $bundle));

      if (!in_array($duration_field, $bundle_fields)) {
        form_set_error('tlearn_class_durationfield', 
          format_string('內容類型: @type 中無此有效天數欄位: @field', 
              array('@type' => $bundle, '@field' => $duration_field)));

        $error = true; 
      }
    }
  }

  
  /* 
   * 課程下載檔案欄位檢查 
   */
  $download_field = $form_state['values']['tlearn_class_downloadfield']; 
  if (!empty($download_field)) {
    $priavate_path = variable_get('file_private_path'); 
    if (empty($priavate_path)) {
      form_set_error('', 
          format_string('在定義課程下載欄位之前，您必需先啟動 Drupal 的 !link', 
              array('!link' => l('私人目錄', 'admin/config/media/file-system')))); 
      $error = true; 
    }
  }
  
  // 檢查 sitefolder 相關設定: tlearn_site_path 
  $folder_path = $form_state['values']['tlearn_site_path']; 
  if (!empty($folder_path)) {
    $form_state['values']['tlearn_site_path'] = trim($folder_path); 
    
    // 是否存在此目錄
    $folder_path = rtrim(trim($folder_path), '/\\');
    
    $folder_uri = 'private://tlearn/' . $folder_path; 
    
    if (!file_prepare_directory($folder_uri)) {
      form_set_error('tlearn_site_path', 
          format_string('網站內部目錄: !path 不正確 (沒有此目錄, 或 drupal 無讀寫權限)',
              array('!path' => $folder_path))); 
      $error = true; 
    }
  }
  
  // AWS 相關設定，在節次設定時，再檢查 
}



/******************************
 * 節次參數設定
 */

function tlearn_admin_section($form, &$form_state) {
  $form = array(); 
  
  $form['markup0'] = array(
    '#markup' => '請先閱讀本頁各項設定細節，在準備完成後，再進行設定。',
  ); 
   
  $form['tlearn_default_options'] = array(
    '#type' => 'fieldset',
    '#title' => '節次基本設定',
    '#collapsible' => FALSE,
    '#description' => 'Tlearn 節次基本設定。',
  );  
  
  $form['tlearn_default_options']['tlearn_section_type'] = array(
    '#type' => 'select',
    '#title' => '節次內容類型', 
    '#required' => true, 
    '#empty_value' => 0, 
    '#options' => tlearn_node_type_options(),
    '#description' => '請選擇節次所使用的內容類型',
    '#default_value' => variable_get('tlearn_section_type', null),    
  );

  $form['tlearn_default_options']['tlearn_section_classref'] = array(
    '#type' => 'select',
    '#title' => '連結課程的連結欄位', 
    '#required' => true, 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇連結課程的連結欄位',
    '#default_value' => variable_get('tlearn_section_classref', null),    
  );


  $form['tlearn_default_options']['tlearn_demoflag_field'] = array(
    '#type' => 'select',
    '#title' => '試讀欄位', 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇定義試讀的欄位。僅可填 1 (免費閱讀), 2 (會員免費閱讀) 或空白。不填的話代表沒有免費閱讀。',
    '#default_value' => variable_get('tlearn_demoflag_field', null),    
  );


  $form['tlearn_default_options']['tlearn_chapord_field'] = array(
    '#type' => 'select',
    '#title' => '章次編號欄位', 
    '#required' => true, 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇章次編號欄位', 
    '#default_value' => variable_get('tlearn_chapord_field', null),    
  );

  $form['tlearn_default_options']['tlearn_sectionord_field'] = array(
    '#type' => 'select',
    '#title' => '節次編號欄位', 
    '#required' => true, 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇章次編號欄位', 
    '#default_value' => variable_get('tlearn_sectionord_field', null),    
  );

  $form['tlearn_default_options']['tlearn_sectionname_field'] = array(
    '#markup' => '節次名稱直接以節次內容類型的標題，作為該節次的名稱。', 
  );

  
  $form['tlearn_moviefield_options'] = array(
    '#type' => 'fieldset',
    '#title' => '影片欄位設定',
    '#collapsible' => FALSE,
    '#description' => '影片欄位設定。',
  );  
  
  $form['tlearn_moviefield_options']['tlearn_youtube_field'] = array(
    '#type' => 'select',
    '#title' => 'Youtube 嵌入描述欄位', 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇 Youtube 嵌入描述的欄位', 
    '#default_value' => variable_get('tlearn_youtube_field', null),    
  );

  $form['tlearn_moviefield_options']['tlearn_free_moviefield'] = array(
    '#type' => 'select',
    '#title' => '免費閱讀的影片檔名欄位', 
    '#required' => true, 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇免費閱讀的影片檔名欄位。無填寫代表沒有提供免費閱讀。', 
    '#default_value' => variable_get('tlearn_free_moviefield', null),    
  );

  $form['tlearn_moviefield_options']['tlearn_memberfree_moviefiled'] = array(
    '#type' => 'select',
    '#title' => '會員免費閱讀的影片檔名欄位', 
    '#required' => true, 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇會員免費閱讀的影片檔名欄位。無填寫代表沒有會員免費閱讀。', 
    '#default_value' => variable_get('tlearn_memberfree_moviefiled', null),    
  );

  $form['tlearn_moviefield_options']['tlearn_authorized_moviefield'] = array(
    '#type' => 'select',
    '#title' => '有權限者的影片檔名欄位', 
    '#required' => true, 
    '#empty_value' => 0, 
    '#options' => tlearn_field_options(),    
    '#description' => '請選擇有權限者的影片檔名欄位', 
    '#default_value' => variable_get('tlearn_authorized_moviefield', null),    
  );

  $form['tlearn_control_options'] = array(
    '#type' => 'fieldset',
    '#title' => '影片控制設定',
    '#collapsible' => FALSE,
    '#description' => 'Tlearn 影片控制設定。',
  );  
  
  $form['tlearn_control_options']['tlearn_cast_free'] = array(
    '#type' => 'radios',
    '#title' => '免費閱讀影片處理', 
    '#options' => array(
      TLEARN_USE_NONE => '無',
      TLEARN_USE_YOUTUBE => '使用 Youtube，會使用 youtube 描述欄位的內容進行播放', 
      TLEARN_USE_SITEFOLDER => '使用網站內目錄播放', 
      TLEARN_USE_CLOUDFRONT => '使用 Cloudfront 播放',
    ), 
    '#default_value' => variable_get('tlearn_cast_free', null),    
  );

  $form['tlearn_control_options']['tlearn_cast_memberfree'] = array(
    '#type' => 'radios',
    '#title' => '會員免費閱讀影片處理', 
    '#options' => array(
      TLEARN_USE_NONE => '無',
      TLEARN_USE_YOUTUBE => '使用 Youtube', 
      TLEARN_USE_SITEFOLDER => '使用網站內目錄播放', 
      TLEARN_USE_CLOUDFRONT => '使用 Cloudfront 播放',
    ), 
    '#default_value' => variable_get('tlearn_cast_memberfree', null),    
  );

  $form['tlearn_control_options']['tlearn_cast_authorized'] = array(
    '#type' => 'radios',
    '#title' => '有權限者閱讀影片處理', 
    '#options' => array(
      TLEARN_USE_NONE => '無',
      TLEARN_USE_YOUTUBE => '使用 Youtube', 
      TLEARN_USE_SITEFOLDER => '使用網站內目錄播放', 
      TLEARN_USE_CLOUDFRONT => '使用 Cloudfront 播放',
    ), 
    '#default_value' => variable_get('tlearn_cast_authorized', null),    
  );

  return system_settings_form($form); 
}


/*
 * 檢查各個欄位是否在指定的節次內容類型中 
 * 
 */
function tlearn_admin_section_validate($form, &$form_state) {
  $error = false; 
  
  $class_types = array_map('trim', explode(',', variable_get('tlearn_class_type'))); 
  
  if (!$class_types || empty($class_types)) {
    form_set_error('', '尚未設定課程，請先設定課程相關設定。');
    return; 
  }

  /*
   * 節次內容類型: tlearn_section_type
   */
  $node_types = array_keys(node_type_get_names());  // keyd by type

  $section_type = $form_state['values']['tlearn_section_type']; 
  if (!in_array($section_type, $node_types)) {
    form_set_error('tlearn_section_type', 
        format_string('"%type" 不是內容類型', array('%type'=> $section_type))); 
    $error = true; 
  }
  
  $section_fields = array_keys(field_info_instances('node', $section_type));
  
  /*
   * 連結課程的連結欄位: tlearn_section_classref
   */
  $classref_field = $form_state['values']['tlearn_section_classref']; 
  
  if (!in_array($classref_field, $section_fields)) {
    form_set_error('tlearn_section_classref', 
      format_string('內容類型: @type 中無此課程節點連結欄位: @field', 
          array('@type' => $section_type, '@field' => $classref_field)));

    $error = true; 
  }
  
  /*
   * 試讀欄位: tlearn_demoflag_field
   */
  $demo_field = $form_state['values']['tlearn_demoflag_field']; 
  
  if (!in_array($demo_field, $section_fields)) {
    form_set_error('tlearn_demoflag_field', 
      format_string('內容類型: @type 中無此展示記號欄位: @field', 
          array('@type' => $section_type, '@field' => $demo_field)));

    $error = true; 
  }
  
  /*
   * 章次編號欄位: tlearn_chapord_field
   */
  $chapord_field = $form_state['values']['tlearn_chapord_field']; 
  
  if (!in_array($chapord_field, $section_fields)) {
    form_set_error('tlearn_chapord_field', 
      format_string('內容類型: @type 中無此章次編號欄位: @field', 
          array('@type' => $section_type, '@field' => $chapord_field)));

    $error = true; 
  }
  
  /*
   * 節次編號欄位: tlearn_sectionord_field
   */
  $sectionord_field = $form_state['values']['tlearn_sectionord_field']; 
  
  if (!in_array($sectionord_field, $section_fields)) {
    form_set_error('tlearn_sectionord_field', 
      format_string('內容類型: @type 中無此節次編號欄位: @field', 
          array('@type' => $section_type, '@field' => $sectionord_field)));

    $error = true; 
  }
  
  // youtube 欄位有用到時，再檢查 
    
  /* 
   * 免費閱讀的影片檔名欄位: tlearn_free_moviefield
   */
  $freemovie_field = $form_state['values']['tlearn_free_moviefield']; 
  
  if (!in_array($freemovie_field, $section_fields)) {
    form_set_error('tlearn_free_moviefield', 
      format_string('內容類型: @type 中無此免費閱讀影片欄位: @field', 
          array('@type' => $section_type, '@field'=>$freemovie_field)));

    $error = true; 
  }
  
  
  /* 
   * 會員免費閱讀的影片檔名欄位: tlearn_memberfree_moviefiled
   */
  $memberfreemovie_field = $form_state['values']['tlearn_memberfree_moviefiled']; 
  
  if (!in_array($memberfreemovie_field, $section_fields)) {
    form_set_error('tlearn_memberfree_moviefiled', 
      format_string('內容類型: @type 中無此會員免費閱讀影片欄位: @field', 
          array('@type' => $section_type, '@field'=>$memberfreemovie_field)));

    $error = true; 
  }
  
  /* 
   * 有權限者的影片檔名欄位: tlearn_authorized_moviefield
   */
  $authorized_field = $form_state['values']['tlearn_authorized_moviefield']; 
  
  if (!in_array($authorized_field, $section_fields)) {
    form_set_error('tlearn_authorized_moviefield', 
      format_string('內容類型: @type 中無此有權限者影片欄位: @field', 
          array('@type' => $section_type, '@field'=>$authorized_field)));

    $error = true; 
  }
    
  /* 
   * 影片控制設定, 這邊才是檢查重點，依設定的方式進行檢查   
   */
  $youtubed = $sitefoldered = $cloudfronted = false; 
  
  $methods = array();
  $methods[] = (int) $form_state['values']['tlearn_cast_free']; 
  $methods[] = (int) $form_state['values']['tlearn_cast_memberfree']; 
  $methods[] = (int) $form_state['values']['tlearn_cast_authorized']; 
  
  foreach ($methods as $method) {
    if (!$youtubed) $youtubed = ($method == TLEARN_USE_YOUTUBE);
    if (!$sitefoldered) $sitefoldered = ($method == TLEARN_USE_SITEFOLDER); 
    if (!$cloudfronted) $cloudfronted = ($method == TLEARN_USE_CLOUDFRONT); 
  }
  
  if ($youtubed) {
    $youtube_field = $form_state['values']['tlearn_youtube_field'];
    
    if (!in_array($youtube_field, $section_fields)) {
      form_set_error('tlearn_youtube_field', 
        format_string('內容類型: @type 中無此 youtube 嵌入欄位: @field', 
            array('@type' => $section_type, '@field'=>$youtube_field)));

      $error = true; 
    }
  }

  if ($sitefoldered) {
    // 檢查 sitefolder 相關設定: tlearn_site_path (非此 form)
    $folder_path = variable_get('tlearn_site_path'); 
    if (empty($folder_path)) {
      form_set_error('', '您還沒有設定課程影片的網站內部目錄，請先至課程參數設定處理。');
      $error = true; 
    }
  }
 
  if ($cloudfronted) {
    // 檢查 cloudfront key file 存在: tlearn_cloudfront_key_filename (非此 form) 

    $keyfile_path = trim(variable_get('tlearn_cloudfront_key_filename')); 
    if (!file_exists($keyfile_path)) {
      form_set_error('', 
        format_string('無法使用 Cloudfront 進行播放。Cloudfront Key File @keyfile 不存在。請先至課程設定進行更正。',
          array('@keyfile' => $keyfile_path)));
      $error = true; 
    }
    
    $keyid = variable_get('tlearn_cloudfront_pair_id', ''); 
    
    if (empty($keyfile_path)) {
      form_set_error('', 
        format_string('無法使用 Cloudfront 進行播放。Cloudfront Key Pair Id 未填寫。請先至課程設定進行更正。')); 
      $error = true; 
    }
  }
}


/******************************
 * 角色權限設定 
 */

/**
 * Menu callback: list admin form with list of available role privileges.
 *
 * @see tlearn_admin_role_submit()
 * @see theme_tlearn_admin_roles()
 * 
 */
function tlearn_admin_troles($form) {
  $form = array(); 
  
  $form['#tree'] = TRUE;
  if (($troles = tlearn_troles_load_multiple())) { 
    foreach ($troles as $trole) {
      $form[$trole->rid]['#role'] = $trole;
      $form[$trole->rid]['name'] = array('#markup' => check_plain($trole->name));
      $form[$trole->rid]['weight'] = array('#type' => 'weight', '#delta' => 10, '#default_value' => $trole->weight);

      $form[$trole->rid]['terms'] = array('#markup' => tlearn_terms_desc($trole->terms));
      
      // 二個操作項目，要放在同一欄，因此後面要呼叫 theme 函數，幫化表格化，同時置放為一欄
      $form[$trole->rid]['edit'] = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => "admin/config/tlearn/troles/$trole->rid/edit",
      );
      $form[$trole->rid]['delete'] = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => "admin/config/tlearn/troles/$trole->rid/delete",
      );
    }
  }

  // Only make this form include a submit button and weight if more than one
  // category exists.
  if (count($troles) > 1) {
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  }
  elseif (!empty($troles)) {
    $form[$trole->rid]['weight'] = array('#type' => 'value', '#value' => 0);
  }
  return $form;
}


/*
 * Menu callback: Role admin form for role add/edit.
 * 增加一個角色的閱讀權限 
 * 
 * @see tlearn_admin_trole_form_validate()
 * @see tlearn_admin_trole_form_submit()
 */
function tlearn_admin_trole_form($form, &$form_state, $trole=array()) {

  if (!empty($trole)) {    
    $form['#trole'] = $trole; 
    $form['trole'] = array(
      '#title' => '角色',
      '#markup' => $trole->name,
    ); 
  }
  else {
    $form['#trole'] = array();
    $form['trole'] = array(
      '#type' => 'select', 
      '#title' => '角色', 
      '#required' => true, 
      '#options' => tlearn_role_options(),
    );
  }
  
  // term 的勾選，包括「全選」，「都不要」
  $form['terms'] = array(
    '#type' => 'checkboxes',
    '#title' => '可閱讀的課程類別',   
    '#required' => true, 
    '#options' => tlearn_term_options(), 
    '#default_value' => empty($trole) ? array() : $trole->terms,
  );  
  
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}




function tlearn_admin_trole_form_validate($form, &$form_state) {
  
}



function tlearn_admin_trole_form_submit($form, &$form_state) {

//  dpm($form_state);
  
  $form_state['redirect'] = 'admin/config/tlearn/troles'; 
  
  $terms = array(); 
  $choices = array_values($form_state['values']['terms']); 
  
  
  if (in_array(TLEARN_OPTION_ALL, $choices)) {
    $terms = array(TLEARN_OPTION_ALL);
  } 
  elseif (in_array(TLEARN_OPTION_NONE, $choices)) {
    $terms = array(TLEARN_OPTION_NONE); 
  }  
  else {
    foreach($choices as $value) {
      if ($value) {
        $terms[] = $value; 
      }
    }
  }
  
  // edit 項目時，抓出原 rid  
  if (!empty($form['#trole'])) {
    $rid = $form['#trole']->rid; 
  }
  else {
    $rid = $form_state['values']['trole']; 
  }
  
  db_merge('tlearn_roles')
    ->key(array('rid' => $rid))
    ->fields(array(
        'terms' => implode(",", $terms),
    ))
    ->execute();   

  drupal_set_message('新增 / 修改角色閱讀權限成功！');   
}


/**
 * Menu callback: Delete trole.
 *
 * @see tlearn_admin_trole_delete_submit()
 */
function tlearn_admin_trole_delete($form, &$form_state, $trole) {
  // Store some category values for submit handling.
  $form = array();
  $form['rid'] = array('#type' => 'value', '#value' => $trole->rid);
  $form['name'] = array('#type' => 'value', '#value' => $trole->name); 

  return confirm_form($form, 
      t('確定要刪除角色 %name 的閱讀權限設定?', array('%name' => $trole->name)), 
      'admin/tlearn/troles',
      t('This action cannot be undone.'), 
      t('Delete'), 
      t('Cancel')
  );
}

/**
 * Form submit callback for deleting a trole setting.
 */
function tlearn_admin_trole_delete_submit($form, &$form_state) {
  $rid = $form_state['values']['rid'];
  $name = $form_state['values']['name'];

  $num_deleted = db_delete('tlearn_roles')
    ->condition('rid', $rid)
    ->execute();  

  drupal_set_message(t('角色 %name 的閱讀權限設定已刪除.', array('%name' => $name)));

  $form_state['redirect'] = 'admin/config/tlearn/troles';
  return;
}


/***********************************
 * 訊息
 */







/***********************************
 * Misc 
 */

function tlearn_admin_message($form, &$form_state) {
    
  $form = array();
  
  $form['tlearn_default_options'] = array(
    '#type' => 'fieldset',
    '#title' => '相關訊息設定',
    '#collapsible' => FALSE,
    '#description' => 'Tlearn 相關訊息設定。',
  );  
  
  $msg = '您目前沒有單一課程的權限';
  $form['tlearn_default_options']['tlearn_msg_nosingleclass'] = array(
    '#type' => 'textarea',
    '#title' => '課程清單中沒有項目時的訊息', 
    '#rows' => 4, 
    '#description' => '',
    '#default_value' => variable_get('tlearn_msg_nosingleclass', $msg),    
  );
  
  
  return system_settings_form($form);
}
